/*
 * trafo.c
 *
 *  Created on: 05.01.2022
 *      Author: Torben Deppe, Julian Grothoff
 *
 *  Collects functions that are necessary to transform ov object to ua nodes via UA_Nodestore as a custom interface.
 */

#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif

#include "trafo.h"
#include "fb_macros.h"
#include "opcua.h"
#include "helpers.h"
#include "nsSwitch.h"

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
transformFunctionChart(
		OV_INSTPTR_fb_functionchart pobj, const UA_Server * server,
		const UA_NodeId * nodeId){
	UA_Node * node = transformGenericObject(server,
			pobj->v_identifier, "My transformed functionchart",
			UA_NODECLASS_OBJECT, nodeId, UA_NODEID_NULL, NULL,
			UA_NS0ID_BASEOBJECTTYPE,
			"BaseObjectType");

	// Add references
	// Organizes .CMD, STATUS and OPERATIONS
	
	// Add STATUS reference as virtual node
	UA_ExpandedNodeId targetNodeId;
	UA_ExpandedNodeId_init(&targetNodeId);
	UA_NodeId_copy(nodeId, &targetNodeId.nodeId);
	opcua_helpers_UA_String_append(&targetNodeId.nodeId.identifier.string, "||STATUS");
	targetNodeId.nodeId.namespaceIndex = OV_OPCUA_DEFAULTNSINDEX;
	opcua_helpers_addReference(node, UA_REFERENCETYPEINDEX_ORGANIZES, targetNodeId, "STATUS", targetNodeId.nodeId.namespaceIndex, UA_TRUE);
	UA_ExpandedNodeId_clear(&targetNodeId);
	
	// Add OPERATIONS reference
	UA_ExpandedNodeId_clear(&targetNodeId);
	UA_ExpandedNodeId_init(&targetNodeId);
	UA_NodeId_copy(nodeId, &targetNodeId.nodeId);
	opcua_helpers_UA_String_append(&targetNodeId.nodeId.identifier.string, "||OPERATIONS");
	targetNodeId.nodeId.namespaceIndex = OV_OPCUA_DEFAULTNSINDEX;
	opcua_helpers_addReference(node, UA_REFERENCETYPEINDEX_ORGANIZES, targetNodeId, "OPERATIONS", targetNodeId.nodeId.namespaceIndex, UA_TRUE);
	UA_ExpandedNodeId_clear(&targetNodeId);
	
	//Add a variable of the functionchart directly under object
	UA_ExpandedNodeId_clear(&targetNodeId);
	UA_ExpandedNodeId_init(&targetNodeId);
	UA_NodeId_copy(nodeId, &targetNodeId.nodeId);
	opcua_helpers_UA_String_append(&targetNodeId.nodeId.identifier.string, ".actimode");
	targetNodeId.nodeId.namespaceIndex = OV_OPCUA_DEFAULTNSINDEX;
	opcua_helpers_addReference(node, UA_REFERENCETYPEINDEX_ORGANIZES, targetNodeId, "actimode", targetNodeId.nodeId.namespaceIndex, UA_TRUE);
	UA_ExpandedNodeId_clear(&targetNodeId);
	return node;
}

static UA_StatusCode
transformVariableValue(UA_VariableNode * node, OV_ANY * value){
UA_StatusCode result = UA_STATUSCODE_GOOD;
	switch(value->value.vartype & OV_VT_KSMASK){
		case OV_VT_ANY:
		case OV_VT_VOID:
			node->valueRank = -3;	//	scalar or one dimension
			node->arrayDimensionsSize = 0;
			node->arrayDimensions = UA_Array_new(node->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	//	scalar or one dimension
			break;
		default:
			if(value->value.vartype & OV_VT_ISVECTOR){
				//	vector
				node->valueRank = 1;
				node->arrayDimensionsSize = 1;
				node->arrayDimensions = UA_Array_new(node->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	//	scalar or one dimension
				if(!node->arrayDimensions){
					result = UA_STATUSCODE_BADOUTOFMEMORY;
					break;
				} else {
					result = UA_STATUSCODE_GOOD;
				}
				result = UA_Array_copy(&value->value.valueunion.val_generic_vec.veclen,
						node->arrayDimensionsSize, (void**)&(node->arrayDimensions), &UA_TYPES[UA_TYPES_INT32]);
			} else {
				//	scalar
				node->valueRank = -1;
				node->arrayDimensionsSize = 0;
				node->arrayDimensions = UA_Array_new(node->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	//	scalar or one dimension
			}
			break;
	}
	value->value.vartype &= OV_VT_KSMASK;
	if(result == UA_STATUSCODE_GOOD){
		result = opcua_helpers_ovAnyToUAVariant(value, &node->value.data.value.value);
		node->valueSource = UA_VALUESOURCE_DATA;
		node->dataType = opcua_helpers_ovVarTypeToNodeId(value->value.vartype);
	}
	node->minimumSamplingInterval = 0;
	node->historizing = UA_FALSE;
	node->value.data.callback.onRead = NULL;
	node->value.data.callback.onWrite = NULL;
	return result;
}

static UA_Node *
transformStatusVariable(const UA_Server * server,
		const UA_NodeId * nodeId, OV_STRING identifier, OV_STRING parentPath){
	ov_memstack_lock();
	OV_STRING virtualParentPath = NULL;
	ov_string_stack_print(&virtualParentPath, "%s||STATUS", parentPath);
	UA_VariableNode * node = (UA_VariableNode*) transformGenericObject(server,
			identifier, "Description", //Currently the dget description from port value
			UA_NODECLASS_VARIABLE, nodeId,
			UA_NODEID_STRING(nodeId->namespaceIndex, virtualParentPath), "STATUS",
			UA_NS0ID_PROPERTYTYPE, "PropertyType");
	ov_memstack_unlock();

	// Resolve parent node id and check that it is a functionchart
	UA_NodeId parentNodeId = UA_NODEID_STRING(nodeId->namespaceIndex, parentPath);
	OV_INSTPTR_ov_object pobj = opcua_helpers_resolveNodeIdToOvObject(&parentNodeId);
	if(pobj == NULL || !Ov_CanCastTo(fb_functionchart, pobj)){
		return (UA_Node*)node;
	}
	OV_INSTPTR_fb_functionchart pFunctionchart = Ov_StaticPtrCast(fb_functionchart, pobj);
	OV_ANY value = OV_ANY_INIT;
	fb_functionchart_getport(pFunctionchart, identifier, &value);
	node->accessLevel = UA_ACCESSLEVELMASK_READ;

	transformVariableValue(node, &value);

	return (UA_Node*)node;
}

static UA_Node *
createStatusFolder(const UA_Server * server,
		const UA_NodeId * nodeId, OV_STRING parentPath, OV_STRING parentBrowseName){
	UA_Node * node = transformGenericObject(server,
			"STATUS", "A virtual folder for all functionchart output ports.",
			UA_NODECLASS_OBJECT, nodeId,
			UA_NODEID_STRING(nodeId->namespaceIndex, parentPath), parentBrowseName,
			UA_NS0ID_FOLDERTYPE, "FolderType");

	// Add status variables as organizes references
	// Resolve parent node id and check that it is a functionchart
	UA_NodeId parentNodeId = UA_NODEID_STRING(nodeId->namespaceIndex, parentPath);
	OV_INSTPTR_ov_object pobj = opcua_helpers_resolveNodeIdToOvObject(&parentNodeId);
	if(pobj == NULL || !Ov_CanCastTo(fb_functionchart, pobj)){
		return node;
	}

	// Set default values for all ports
	UA_ExpandedNodeId targetNodeId;

	// Iterate over output ports
	OV_INSTPTR_fb_functionchart pFunctionchart = Ov_StaticPtrCast(fb_functionchart, pobj);
	OV_INSTPTR_fb_port pport = NULL;
	OV_STRING portPath = NULL;
	ov_memstack_lock();
	Ov_ForEachChild(fb_variables, pFunctionchart, pport){
		//List all outputs
		if(IsFlagSet(pport->v_flags, 'o')){
			//Add organizes reference for every output port
			ov_string_stack_print(&portPath, "%s||STATUS||%s", parentPath, pport->v_identifier);
			targetNodeId = UA_EXPANDEDNODEID_STRING_ALLOC(nodeId->namespaceIndex,
					portPath);
			opcua_helpers_addReference(node, UA_REFERENCETYPEINDEX_HASPROPERTY, targetNodeId, "STATUS", targetNodeId.nodeId.namespaceIndex, UA_TRUE);
			UA_ExpandedNodeId_clear(&targetNodeId);
		}
	}
	ov_memstack_unlock();
	return node;
}

static OV_RESULT
getPortValue(const UA_NodeId* nodeId, OV_STRING functionchartPath, OV_STRING portName, OV_ANY* portValue, OV_INSTPTR_fb_port* pPort){
	// Resolve parent node id and check that it is a functionchart
	UA_NodeId parentNodeId = UA_NODEID_STRING(nodeId->namespaceIndex, functionchartPath);
	OV_INSTPTR_ov_object pobj = opcua_helpers_resolveNodeIdToOvObject(&parentNodeId);
	if(pobj != NULL || !Ov_CanCastTo(fb_functionchart, pobj)){
		if(pPort){
			OV_INSTPTR_fb_port pvar;
			Ov_ForEachChild (fb_variables, Ov_StaticPtrCast(fb_functionchart, pobj), pvar)
			{
				if (ov_string_compare (pvar->v_identifier, portName) == OV_STRCMP_EQUAL)
				{
					*pPort =pvar;
					break;
				}
			}
		}
		if(portValue != NULL)
			return fb_functionchart_getport(Ov_StaticPtrCast(fb_functionchart, pobj), portName, portValue);
		else
			return OV_ERR_OK;
	}
	return OV_ERR_GENERIC;
}

static UA_Node *
transformMethodArguments(const UA_Server * server,
		const UA_NodeId * nodeId, OV_STRING methodIdentifier, OV_STRING identifier, OV_STRING parentPath){
	if(ov_string_compare(identifier, "OutputArguments") == OV_STRCMP_EQUAL)
		return NULL; //Not implemented

	ov_memstack_lock();
	OV_STRING virtualParentPath = NULL;
	ov_string_stack_print(&virtualParentPath, "%s||OPERATIONS||%s", parentPath, methodIdentifier);
	UA_VariableNode * node = (UA_VariableNode*) transformGenericObject(server,
			identifier, "The set method has only one input argument, which corresponds to the input port of the functionchart.",
			UA_NODECLASS_VARIABLE, nodeId,
			UA_NODEID_STRING(nodeId->namespaceIndex, virtualParentPath), methodIdentifier,
			UA_NS0ID_PROPERTYTYPE, "PropertyType");
	node->head.browseName.namespaceIndex = 0;
	ov_memstack_unlock();

	OV_ANY parameterPort = OV_ANY_INIT;
	OV_INSTPTR_fb_port pPort = NULL;
	if(getPortValue(nodeId, parentPath, methodIdentifier, &parameterPort, &pPort) == OV_ERR_OK){
		node->accessLevel = UA_ACCESSLEVELMASK_READ;
		node->dataType = UA_NODEID_NUMERIC(0, UA_NS0ID_ARGUMENT);
		node->minimumSamplingInterval = 0;
		node->historizing = UA_FALSE;
		node->valueSource = UA_VALUESOURCE_DATA;

		node->value.data.callback.onRead = NULL;
		node->value.data.callback.onWrite = NULL;
		UA_DataValue_init(&node->value.data.value);

		OV_UINT sizeInput = 1;
		// arrayDimensions
		node->arrayDimensionsSize = 1;
		node->arrayDimensions = UA_UInt32_new();
		node->arrayDimensions[0] = sizeInput;
		node->valueRank = UA_VALUERANK_ONE_DIMENSION;

		// value
		UA_Variant* value = ((UA_Variant*)&node->value.data.value.value);
		value->type = &UA_TYPES[UA_TYPES_ARGUMENT];
		value->arrayLength = sizeInput;
		if (sizeInput > 1){
			value->data = UA_Array_new(sizeInput, &UA_TYPES[UA_TYPES_ARGUMENT]);
		}else{
			value->data = UA_Argument_new();
		}

		node->value.data.value.hasValue = TRUE;
		node->valueSource = UA_VALUESOURCE_DATA;
		int count = sizeInput-1;

		UA_Argument_init(&((UA_Argument*)value->data)[count]);
		((UA_Argument*)value->data)[count].description = UA_LOCALIZEDTEXT_ALLOC("en_US", pPort->v_comment);
		((UA_Argument*)value->data)[count].name = UA_STRING_ALLOC(methodIdentifier);
		switch(parameterPort.value.vartype){
			case OV_VT_BOOL:
				((UA_Argument*)value->data)[count].dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
				((UA_Argument*)value->data)[count].valueRank = UA_VALUERANK_SCALAR;
				break;
			case OV_VT_INT:
				((UA_Argument*)value->data)[count].dataType = UA_TYPES[UA_TYPES_INT32].typeId;
				((UA_Argument*)value->data)[count].valueRank = UA_VALUERANK_SCALAR;
				break;
			case OV_VT_UINT:
				((UA_Argument*)value->data)[count].dataType = UA_TYPES[UA_TYPES_UINT32].typeId;
				((UA_Argument*)value->data)[count].valueRank = UA_VALUERANK_SCALAR;
				break;
			case OV_VT_SINGLE:
				((UA_Argument*)value->data)[count].dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
				((UA_Argument*)value->data)[count].valueRank = UA_VALUERANK_SCALAR;
				break;
			case OV_VT_DOUBLE:
				((UA_Argument*)value->data)[count].dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
				((UA_Argument*)value->data)[count].valueRank = UA_VALUERANK_SCALAR;
				break;
			case OV_VT_ANY:
				((UA_Argument*)value->data)[count].dataType = UA_TYPES[UA_TYPES_VARIANT].typeId;
				((UA_Argument*)value->data)[count].valueRank = UA_VALUERANK_SCALAR_OR_ONE_DIMENSION;
				//((UA_Argument*)value->data)[count].arrayDimensionsSize = ; // get from tmpPart.pvalue
				//((UA_Argument*)value->data)[count].arrayDimensions = UA_UInt32_new();
				//((UA_Argument*)value->data)[count].arrayDimensions[0] = tmpPart.elemunion.pvar->v_veclen;
				break;
			case OV_VT_STRING:
				((UA_Argument*)value->data)[count].dataType = UA_TYPES[UA_TYPES_STRING].typeId;
				((UA_Argument*)value->data)[count].valueRank = UA_VALUERANK_SCALAR;
				break;
			case OV_VT_BOOL_VEC:
				((UA_Argument*)value->data)[count].dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
				((UA_Argument*)value->data)[count].valueRank = UA_VALUERANK_ONE_DIMENSION;
				break;
			case OV_VT_INT_VEC:
				((UA_Argument*)value->data)[count].dataType = UA_TYPES[UA_TYPES_INT32].typeId;
				((UA_Argument*)value->data)[count].valueRank = UA_VALUERANK_ONE_DIMENSION;
				break;
			case OV_VT_UINT_VEC:
				((UA_Argument*)value->data)[count].dataType = UA_TYPES[UA_TYPES_UINT32].typeId;
				((UA_Argument*)value->data)[count].valueRank = UA_VALUERANK_ONE_DIMENSION;
				break;
			case OV_VT_SINGLE_VEC:
				((UA_Argument*)value->data)[count].dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
				((UA_Argument*)value->data)[count].valueRank = UA_VALUERANK_ONE_DIMENSION;
				break;
			case OV_VT_DOUBLE_VEC:
				((UA_Argument*)value->data)[count].dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
				((UA_Argument*)value->data)[count].valueRank = UA_VALUERANK_ONE_DIMENSION;
				break;
			case OV_VT_STRING_VEC:
				((UA_Argument*)value->data)[count].dataType = UA_TYPES[UA_TYPES_STRING].typeId;
				((UA_Argument*)value->data)[count].valueRank = UA_VALUERANK_ONE_DIMENSION;
				break;
			default:
				break;
		}
		if(((UA_Argument*)value->data)[count].valueRank == UA_VALUERANK_SCALAR){
			((UA_Argument*)value->data)[count].arrayDimensionsSize = 0;
			((UA_Argument*)value->data)[count].arrayDimensions = NULL;
		}else if(((UA_Argument*)value->data)[count].valueRank == UA_VALUERANK_ONE_DIMENSION){
			((UA_Argument*)value->data)[count].arrayDimensionsSize = 1;
			((UA_Argument*)value->data)[count].arrayDimensions = UA_UInt32_new();
			((UA_Argument*)value->data)[count].arrayDimensions[0] = parameterPort.value.valueunion.val_bool_vec.veclen;
		}
	}

	return (UA_Node*)node;
}

static UA_StatusCode
methodCallback(UA_Server *server, const UA_NodeId *sessionId,
                     void *sessionContext, const UA_NodeId *methodId,
                     void *methodContext, const UA_NodeId *objectId,
                     void *objectContext, size_t inputSize,
                     const UA_Variant *input, size_t outputSize,
                     UA_Variant *output){
	// Get parent node id and ordername by splitting the methodId string
	if(methodId->identifierType != UA_NODEIDTYPE_STRING)
		return UA_STATUSCODE_BADNODEIDINVALID;
	OV_STRING path = NULL;
	opcua_helpers_copyUAStringToOV(methodId->identifier.string, &path);
	OV_UINT pathLength = 0;
	OV_STRING* pathList = ov_string_split(path, "||", &pathLength);
	ov_string_setvalue(&path, NULL);
	if(pathLength < 3){
		ov_string_freelist(pathList);
		return UA_STATUSCODE_BADNODEIDUNKNOWN;
	}

	UA_NodeId componentId = UA_NODEID_STRING(methodId->namespaceIndex, pathList[pathLength-3]);
	// Resolve parent node id and check that it is a functionchart
	OV_INSTPTR_ov_object pobj = opcua_helpers_resolveNodeIdToOvObject(&componentId);
	if(pobj == NULL || !Ov_CanCastTo(fb_functionchart, pobj)){
		ov_string_freelist(pathList);
		return UA_STATUSCODE_BADTYPEMISMATCH;
	}
	OV_INSTPTR_fb_functionchart pFunctionchart = Ov_StaticPtrCast(fb_functionchart, pobj);

	UA_StatusCode result = UA_STATUSCODE_GOOD;
	if(inputSize == 1){
		OV_ANY portValue = OV_ANY_INIT;
		result = opcua_helpers_UAVariantToOVAny(&input[0], &portValue);
		if(result == UA_STATUSCODE_GOOD){
			result = opcua_helpers_ovResultToUaStatusCode(fb_functionchart_setport(pFunctionchart, pathList[pathLength-1], &portValue));
		}
		Ov_SetAnyValue(&portValue, NULL);
	}else{
		result = UA_STATUSCODE_BADARGUMENTSMISSING;
	}
	ov_string_freelist(pathList);
	return result;
}

static UA_Node *
transformMethod(const UA_Server * server,
		const UA_NodeId * nodeId, OV_STRING identifier, OV_STRING parentPath){
	ov_memstack_lock();
	OV_STRING virtualParentPath = NULL;
	ov_string_stack_print(&virtualParentPath, "%s||OPERATIONS", parentPath);
	UA_MethodNode * node = (UA_MethodNode*) transformGenericObject(server,
			identifier, "Set function for functionchart input port with the same name.",
			UA_NODECLASS_METHOD, nodeId,
			UA_NODEID_STRING(nodeId->namespaceIndex, virtualParentPath), "OPERATIONS",
			0, NULL);
	
	// Add input arguments
	if(getPortValue(nodeId, identifier, parentPath, NULL, NULL) == OV_ERR_OK) {
		UA_ExpandedNodeId targetNodeId;
		UA_ExpandedNodeId_init(&targetNodeId);
		UA_NodeId_copy(nodeId, &targetNodeId.nodeId);
		opcua_helpers_UA_String_append(&targetNodeId.nodeId.identifier.string, "||InputArguments");
		UA_QualifiedName targetBrowseNameQualified = UA_QUALIFIEDNAME(0, "InputArguments");
		UA_Node_addReference((UA_Node*)node, UA_REFERENCETYPEINDEX_HASPROPERTY, UA_TRUE, &targetNodeId, UA_QualifiedName_hash(&targetBrowseNameQualified));
		UA_ExpandedNodeId_clear(&targetNodeId);
	}
	ov_memstack_unlock();

	node->executable = UA_TRUE;
	node->method = methodCallback;
	return (UA_Node*)node;
}

static UA_Node *
createOperationsFolder(const UA_Server * server,
		const UA_NodeId * nodeId, OV_STRING parentPath, OV_STRING parentBrowseName){
	UA_Node * node = transformGenericObject(server,
			"OPERATIONS", "Virtual folder that contains a set method for each functionchart input port.",
			UA_NODECLASS_OBJECT, nodeId,
			UA_NODEID_STRING(nodeId->namespaceIndex, parentPath), parentBrowseName,
			UA_NS0ID_FOLDERTYPE, "FolderType");

	// Add operations folder via organizes reference
	// Resolve parent node id and check that it is a functionchart
	UA_NodeId parentNodeId = UA_NODEID_STRING(nodeId->namespaceIndex, parentPath);
	OV_INSTPTR_ov_object pobj = opcua_helpers_resolveNodeIdToOvObject(&parentNodeId);
	if(pobj == NULL || !Ov_CanCastTo(fb_functionchart, pobj)){
		return node;
	}

	// Set default values for all operations
	UA_ExpandedNodeId targetNodeId;

	// Iterate over input ports
	OV_INSTPTR_fb_functionchart pFunctionchart = Ov_StaticPtrCast(fb_functionchart, pobj);
	OV_INSTPTR_fb_port pport = NULL;
	OV_STRING portPath = NULL;
	ov_memstack_lock();
	Ov_ForEachChild(fb_variables, pFunctionchart, pport){
		if(IsFlagSet(pport->v_flags, 'i')){
			//Add has component reference for every input port
			ov_string_stack_print(&portPath, "%s||OPERATIONS||%s", parentPath, pport->v_identifier);
			targetNodeId = UA_EXPANDEDNODEID_STRING_ALLOC(nodeId->namespaceIndex,
					portPath);
			opcua_helpers_addReference(node, UA_REFERENCETYPEINDEX_HASCOMPONENT, targetNodeId, "OPERATIONS", targetNodeId.nodeId.namespaceIndex, UA_TRUE);
			UA_ExpandedNodeId_clear(&targetNodeId);
		}
	}
	ov_memstack_unlock();
	return node;
}


/*
 * Nodestore API functions
 */
static const UA_Node * getNode(void * context, const UA_NodeId *nodeId){
	UA_Node * 						node = NULL;
	OV_INSTPTR_ov_object			pobj = NULL;
	OV_INSTPTR_demo_interface 		pinterface = Ov_StaticPtrCast(demo_interface, context);
	OV_INSTPTR_opcua_server 		server = (pinterface) ? Ov_GetParent(opcua_serverToInterfaces, pinterface) : NULL;
	OPCUA_PTR_UA_Server 			uaServer = (server) ? server->v_server : NULL;

	// Check path for virtual node
	if(nodeId->identifierType == UA_NODEIDTYPE_STRING){
		OV_STRING path = NULL;
		OV_UINT length = 0;
		opcua_helpers_copyUAStringToOV(nodeId->identifier.string , &path);
		OV_STRING* pathList = ov_string_split(path, "||", &length);
		if(length > 1){
			if(ov_string_compare(pathList[1], "STATUS") == OV_STRCMP_EQUAL){
				if(length == 2){
					// pathToFunctionChart||STATUS
					// Get the name of the functionchart as the parent browsename
					OV_UINT pathListOVlength = 0;
					OV_STRING* pathListOV = ov_string_split(pathList[0], "/", &pathListOVlength);
					node = createStatusFolder(uaServer, nodeId, pathList[0], pathListOV[pathListOVlength-1]);
					ov_string_freelist(pathListOV);
				}else if(length == 3){
					// pathToFunctionChart||STATUS||VariableName
					node = transformStatusVariable(uaServer, nodeId, pathList[2], pathList[0]);
				}
			}
			if(ov_string_compare(pathList[1], "OPERATIONS") == OV_STRCMP_EQUAL){
				if(length == 2){
					OV_UINT pathListOVlength = 0;
					OV_STRING* pathListOV = ov_string_split(pathList[0], "/", &pathListOVlength);
					node = createOperationsFolder(uaServer, nodeId, pathList[0], pathListOV[pathListOVlength-1]);
					ov_string_freelist(pathListOV);
				}else if(length == 3){
					node = transformMethod(uaServer, nodeId, pathList[2], pathList[0]);
				}
				else if(length == 4){
					node = transformMethodArguments(uaServer, nodeId, pathList[2], pathList[3], pathList[0]);
				}
			}
		}
		ov_string_setvalue(&path, NULL);
		ov_string_freelist(pathList);
	}

	// Check for none virtual node
	if(node == NULL){
		//Resolve nodeId
		pobj = opcua_helpers_resolveNodeIdToOvObject(nodeId);
		if(pobj == NULL){
			return NULL;
		}

		// Visualize every functionchart
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
	UA_Node* node = (UA_Node*) getNode(context, nodeId);
	if(node == NULL)
		return UA_STATUSCODE_BADNODEIDUNKNOWN;
	//No need to copy the node, as they are only temporary constructed
	*nodeOut = node;
	return UA_STATUSCODE_GOOD;
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
trafo_new(OV_INSTPTR_demo_interface context) {
	// Allocate memory for the interface to the nodestore
	UA_Nodestore* ns = (UA_Nodestore*)UA_malloc(sizeof(UA_Nodestore));
	if(ns == NULL)
		return NULL;

	// The context holds a pointer to the demo_interface itselft, to be used inside all other function
	// The server can be accessed via opcua_serverToInterfaces association
    ns->context = 		context;

	// Get a immutable node, and release it if not used anymor
	// This is the main function, that needs to be implemented for unidirectional transformations
	// from OV to OPC UA (without OPC UA nodemanagement services and back transformation)
    ns->getNode =		getNode;
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
trafo_delete(UA_Nodestore *nodestore){
	nodestore->context = NULL;
	UA_free(nodestore);
}