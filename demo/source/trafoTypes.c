/*
 * trafoTypes.c
 *
 *  Created on: 05.01.2022
 *      Author: Torben Deppe, Julian Grothoff
 *
 *  Collects functions that are necessary to transform ov object to ua nodes via UA_Nodestore as a custom interface.
 */

#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif

#include "trafoTypes.h"
#include "fb_macros.h"
#include "fb_namedef.h"
#include "opcua.h"
#include "helpers.h"
#include "nsSwitch.h"
#include "nodeset_generated_handling.h"
#include "nodeset_ids.h"

/*
 * Static helper functions to create or transform functioncharts and parts.
 * All used by the getNode nodestore function.
 */

static void
addTypeDefinitionReference(const UA_Server * server, UA_Node* node, const UA_NodeId* nodeId, OV_STRING browseName,
		const UA_ExpandedNodeId typeNodeId, OV_STRING typeBrowseName){
	// hasTypeDefinition reference forward
	opcua_helpers_addReference(node, UA_REFERENCETYPEINDEX_HASTYPEDEFINITION,
			typeNodeId, typeBrowseName, typeNodeId.nodeId.namespaceIndex, UA_TRUE);

	// hasTypeDefinition reference backward
	if(server == NULL)
		return;
	// Get the node from nodestore edit and replace
	UA_ServerConfig* config = UA_Server_getConfig((UA_Server*) server);
	UA_Nodestore* ns =  UA_Nodestore_Switch_Interface_get((UA_Nodestore_Switch *)config->nodestore.context);
	UA_Node * targetNode = NULL;
	ns->getNodeCopy(ns->context, &typeNodeId.nodeId, &targetNode);
	if(targetNode){
		UA_ExpandedNodeId targetNodeId;
		UA_ExpandedNodeId_init(&targetNodeId);
		UA_NodeId_copy(nodeId, &targetNodeId.nodeId);
		if(opcua_helpers_addReference(targetNode, UA_REFERENCETYPEINDEX_HASTYPEDEFINITION,
			targetNodeId, browseName, targetNodeId.nodeId.namespaceIndex, UA_FALSE) == UA_STATUSCODE_GOOD){
			ns->replaceNode(ns->context, targetNode);
		}else{
			ns->deleteNode(ns->context, targetNode);
		}
		UA_ExpandedNodeId_clear(&targetNodeId);
	}
}

static UA_Node *
createNode(UA_NodeClass nodeClass, const OV_STRING identifier, const UA_UInt16 nsIndex){
	UA_Node * node = NULL;
	switch(nodeClass){
	case UA_NODECLASS_VARIABLE:
		node = (UA_Node*) UA_calloc(1, sizeof(UA_VariableNode));
		break;
	case UA_NODECLASS_METHOD:
		node = (UA_Node*) UA_calloc(1, sizeof(UA_MethodNode));
		break;
	case UA_NODECLASS_OBJECT:
	default:
		node = (UA_Node*) UA_calloc(1, sizeof(UA_ObjectNode));
		nodeClass = UA_NODECLASS_OBJECT;
		((UA_ObjectNode*)node)->eventNotifier = 0;
		break;
	}
	node->head.nodeClass = nodeClass;

	// BrowseName
	UA_QualifiedName_init(&node->head.browseName);
	node->head.browseName.name = UA_String_fromChars(identifier);
	node->head.browseName.namespaceIndex = nsIndex;

	// DisplayName
	UA_LocalizedText_init(&node->head.displayName);
	node->head.displayName.locale = UA_String_fromChars("en");
	node->head.displayName.text = UA_String_fromChars(identifier);

	return (UA_Node*)node;
}

static UA_Node *
transformGenericObject(
		const UA_Server * server,
		const OV_STRING identifier, const OV_STRING description, UA_NodeClass nodeClass,
		const UA_NodeId * nodeId, const UA_NodeId parentNodeId, OV_STRING parentBrowseName,
		const UA_UInt32 typeNodeIdNumeric, OV_STRING typeBrowseName){
	UA_Node * node = createNode(nodeClass, identifier, nodeId->namespaceIndex);

	// Description
	UA_LocalizedText_init(&node->head.description);
	node->head.description.locale = UA_String_fromChars("en");
	node->head.description.text = UA_String_fromChars(description);

	// Writemask
	node->head.writeMask = UA_ACCESSLEVELMASK_READ;

	// Add references
	// Parent reference
	if(UA_NodeId_isNull(&parentNodeId) == UA_FALSE)
	{
		UA_ExpandedNodeId targetNodeId;
		UA_ExpandedNodeId_init(&targetNodeId);
		UA_NodeId_copy(&parentNodeId, &targetNodeId.nodeId);

		UA_Byte parentRefTypIndex = (node->head.nodeClass == UA_NODECLASS_METHOD ?
			UA_REFERENCETYPEINDEX_HASCOMPONENT : (node->head.nodeClass == UA_NODECLASS_VARIABLE) ?
				UA_REFERENCETYPEINDEX_HASPROPERTY  : UA_REFERENCETYPEINDEX_ORGANIZES);
		opcua_helpers_addReference(node, parentRefTypIndex, targetNodeId, parentBrowseName, targetNodeId.nodeId.namespaceIndex, UA_FALSE);
		UA_ExpandedNodeId_clear(&targetNodeId);
	}
	// Type definition reference
	if(typeNodeIdNumeric != 0){
		addTypeDefinitionReference(server, node, nodeId, identifier, UA_EXPANDEDNODEID_NUMERIC(0, typeNodeIdNumeric), typeBrowseName);
	}
	return node;
}

static UA_Node *
transformId(const UA_Server * server, const UA_NodeId * nodeId, OV_STRING parentPath){
	OV_INSTPTR_ov_object pParent = ov_path_getobjectpointer(parentPath, 2);
	UA_VariableNode * node = (UA_VariableNode*) transformGenericObject(server,
			"id", "Virtual id variable, which is a struct containing idH and idL of an ov object.",
			UA_NODECLASS_VARIABLE, nodeId,
			UA_NODEID_STRING(nodeId->namespaceIndex, parentPath), pParent->v_identifier,
			UA_NS0ID_PROPERTYTYPE, "PropertyType");

	//Set datatype node id, which is our Identifier datatype node from the nodeset
	node->dataType = UA_NODEID_NUMERIC(2, UA_NS2ID_IDENTIFIERTYPE);

	//Get value and transform to OPC UA
	UA_Variant* pVariant = &node->value.data.value.value;
	UA_Variant_init(pVariant);
	pVariant->type = &UA_NODESET[UA_NODESET_IDENTIFIERTYPE];
	pVariant->data = UA_IdentifierType_new();
	UA_IdentifierType id;
	id.idH = pParent->v_idH;
	id.idL = pParent->v_idL;
	UA_IdentifierType_copy(&id, pVariant->data);

	//Set other variable attributes
	node->accessLevel = UA_ACCESSLEVELMASK_READ;

	node->valueRank = -1;
	node->arrayDimensionsSize = 0;
	node->arrayDimensions = UA_Array_new(node->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	//	scalar or one dimension
	
	node->valueSource = UA_VALUESOURCE_DATA;
	node->value.data.callback.onRead = NULL;
	node->value.data.callback.onWrite = NULL;

	node->minimumSamplingInterval = 0;
	node->historizing = UA_FALSE;
	return (UA_Node*)node;
}

static UA_Node *
transformActimode(const UA_Server * server,	const UA_NodeId * nodeId, OV_STRING parentPath){
	OV_INSTPTR_ov_object pParent = ov_path_getobjectpointer(parentPath, 2);
	OV_INSTPTR_fb_functionchart pChart = Ov_StaticPtrCast(fb_functionchart, pParent);
	UA_VariableNode * node = (UA_VariableNode*) transformGenericObject(server,
			"actimode", "Transformed actimode from fb_task class, with value as enumeration.",
			UA_NODECLASS_VARIABLE, nodeId,
			UA_NODEID_STRING(nodeId->namespaceIndex, parentPath), pParent->v_identifier,
			UA_NS0ID_PROPERTYTYPE, "PropertyType");

	//Set datatype node id, which is our Actimode datatype node from the nodeset
	node->dataType = UA_NODEID_NUMERIC(2, UA_NS2ID_ACTIMODETYPE);

	//Get value and transform to OPC UA
	UA_Variant* pVariant = &node->value.data.value.value;
	UA_Variant_init(pVariant);
	pVariant->type = &UA_NODESET[UA_NODESET_ACTIMODETYPE];
	pVariant->data = UA_ActimodeType_new();
	UA_ActimodeType actimode = FB_AM_OFF;
	if(pChart->v_actimode == FB_AM_ON) actimode = UA_ACTIMODETYPE_ON;
	else if (pChart->v_actimode == FB_AM_UNLINK) actimode = UA_ACTIMODETYPE_UNLINK;
	else if (pChart->v_actimode == FB_AM_ONCE) actimode = UA_ACTIMODETYPE_ONCE;
	else if (pChart->v_actimode == FB_AM_CATCHUP) actimode = UA_ACTIMODETYPE_CATCHUP;
	UA_ActimodeType_copy(&actimode, pVariant->data);

	//Set other variable attributes
	node->accessLevel = UA_ACCESSLEVELMASK_READ;

	node->valueRank = -1;
	node->arrayDimensionsSize = 0;
	node->arrayDimensions = UA_Array_new(node->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	//	scalar or one dimension
	
	node->valueSource = UA_VALUESOURCE_DATA;
	node->value.data.callback.onRead = NULL;
	node->value.data.callback.onWrite = NULL;

	node->minimumSamplingInterval = 0;
	node->historizing = UA_FALSE;
	return (UA_Node*)node;
}

static UA_Node *
transformFunctionChart(OV_INSTPTR_fb_functionchart pobj, const UA_Server * server,	const UA_NodeId * nodeId){
	UA_Node * node = transformGenericObject(server,
			pobj->v_identifier, "My transformed functionchart",
			UA_NODECLASS_OBJECT, nodeId, UA_NODEID_NULL, NULL,
			UA_NS0ID_BASEOBJECTTYPE,
			"BaseObjectType");

	// Add references to virtual id variable
	UA_ExpandedNodeId targetNodeId;
	UA_ExpandedNodeId_init(&targetNodeId);
	UA_NodeId_copy(nodeId, &targetNodeId.nodeId);
	opcua_helpers_UA_String_append(&targetNodeId.nodeId.identifier.string, "||id");
	targetNodeId.nodeId.namespaceIndex = OV_OPCUA_DEFAULTNSINDEX;
	opcua_helpers_addReference(node, UA_REFERENCETYPEINDEX_HASPROPERTY, targetNodeId, "id", 2, UA_TRUE);
	UA_ExpandedNodeId_clear(&targetNodeId);
	
	//Add the actimode variable of the functionchart
	UA_ExpandedNodeId_clear(&targetNodeId);
	UA_ExpandedNodeId_init(&targetNodeId);
	UA_NodeId_copy(nodeId, &targetNodeId.nodeId);
	opcua_helpers_UA_String_append(&targetNodeId.nodeId.identifier.string, "||actimode");
	targetNodeId.nodeId.namespaceIndex = OV_OPCUA_DEFAULTNSINDEX;
	opcua_helpers_addReference(node, UA_REFERENCETYPEINDEX_HASPROPERTY, targetNodeId, "actimode", 2, UA_TRUE);
	UA_ExpandedNodeId_clear(&targetNodeId);
	return node;
}

/*
 * Nodestore API functions
 */
static const UA_Node * getNode(void * context, const UA_NodeId *nodeId,
                               UA_UInt32 attributeMask,
                               UA_ReferenceTypeSet references,
                               UA_BrowseDirection referenceDirections){
	UA_Node * 						node = NULL;
	OV_INSTPTR_ov_object			pobj = NULL;
	OV_INSTPTR_demo_interfaceTypes 	pinterface = Ov_StaticPtrCast(demo_interfaceTypes, context);
	OV_INSTPTR_opcua_server 		server = (pinterface) ? Ov_GetParent(opcua_serverToInterfaces, pinterface) : NULL;
	OPCUA_PTR_UA_Server 			uaServer = (server) ? server->v_server : NULL;

	if(nodeId->identifierType != UA_NODEIDTYPE_STRING){
		return NULL;
	}

	// Check path for virtual node
	// here only the "id" and "actimode" variable can be found
	OV_STRING path = NULL;
	opcua_helpers_copyUAStringToOV(nodeId->identifier.string , &path);
	OV_UINT length = 0;
	OV_STRING* pathList = ov_string_split(path, "||", &length);
	if(length == 2){
		if(ov_string_compare(pathList[1], "id") == OV_STRCMP_EQUAL)
			node = transformId(uaServer, nodeId, pathList[0]);
		else if(ov_string_compare(pathList[1], "actimode") == OV_STRCMP_EQUAL)
			node = transformActimode(uaServer, nodeId, pathList[0]);
	}
	ov_string_setvalue(&path, NULL);
	ov_string_freelist(pathList);

	// Check for none virtual node
	if(node == NULL){
		//Resolve nodeId
		pobj = opcua_helpers_resolveNodeIdToOvObject(nodeId);
		if(pobj == NULL){
			return NULL;
		}
		// Traform functioncharts
		if(Ov_CanCastTo(fb_functionchart, pobj)){
			node = transformFunctionChart(Ov_StaticPtrCast(fb_functionchart, pobj), uaServer, nodeId);
		}
	}

	// Set nodeId
	if(node != NULL)
		UA_NodeId_copy(nodeId, &node->head.nodeId);

	return node;
}

static void releaseNode(void *context, const UA_Node *node){
	// All our nodes are only constructed temporary on access of the nodestore.
	// So we can safely delete them, if they aren't used anymore (after getNode was called).
	if (node){
		UA_Node_clear((UA_Node *)node);
		UA_free((UA_Node *)node);
	}
}

static UA_StatusCode getNodeCopy(void *context, const UA_NodeId *nodeId, UA_Node ** nodeOut){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

static UA_StatusCode replaceNode(void *context, UA_Node *node){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

static UA_StatusCode removeNode(void *context, const UA_NodeId *nodeId){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

static UA_Node * newNode(void * context, UA_NodeClass nodeClass){
    return NULL;
}

static UA_StatusCode insertNode(void *context, UA_Node *node, UA_NodeId *parrentNode){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

static void deleteNode(void * context, UA_Node *node){
}

static void iterate(void *context, UA_NodestoreVisitor visitor, void* visitorHandle){
}

static void clear(void *context){
}

/*
 * Lifecycle of the trafo as nodestore (interface) itself 
 */
UA_Nodestore*
trafoTypes_new(OV_INSTPTR_demo_interfaceTypes context) {
	// Allocate memory for the interface to the nodestore
	UA_Nodestore* ns = (UA_Nodestore*)UA_malloc(sizeof(UA_Nodestore));
	if(ns == NULL)
		return NULL;

	// The context holds a pointer to the demo_interfaceTypes itselft, to be used inside all other function
	// The server can be accessed via opcua_serverToInterfaces association
    ns->context = 		context;

	// Get a immutable node, and release it if not used anymore
	// This is the main function, that needs to be implemented for unidirectional transformations
	// from OV to OPC UA (without OPC UA nodemanagement services and back transformation)
    ns->getNode =		getNode;
	ns->getNodeFromPtr =NULL; // redirected to getNode by nsSwitch
    ns->releaseNode =	releaseNode;
	// Needed to change OV values, e.g. to write variable values
	// The UA_Server obtaines a copy, changes it and calls replaceNode
	// (c.f. define in open62541 UA_ENABLE_IMMUTABLE_NODES) 
    ns->getNodeCopy =	getNodeCopy;
    ns->replaceNode = 	replaceNode;
    
	// Remove an existing node
	// needed if nodemanagement delete service from OPC UA should be supported
	ns->removeNode =	removeNode;

	// Create a new node and insert it to the nodestore,
	// needed if nodemanagement add service from OPC UA should be supported
    ns->newNode =   	newNode;
    ns->insertNode =  	insertNode;
    ns->deleteNode =	deleteNode;
    
	// Function to iterate over all nodes (e.g. to backup the nodestore state)
	// Currently not used by the UA_Server
	ns->iterate =     	iterate;
	// Delete all members of the nodestore
    ns->clear =			clear;
	
	// Not needed, handled by the nsSwitch in a central manner
	ns->getReferenceTypeId = NULL; 	
    return ns;
}

void
trafoTypes_delete(UA_Nodestore *nodestore){
	nodestore->context = NULL;
	UA_free(nodestore);
}