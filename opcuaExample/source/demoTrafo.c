/*
 * demoTrafo.c
 *
 *  Created on: 06.12.2016
 *      Author: Torben Deppe, Julian Grothoff
 *
 *  Collects functions that are necessary to transform ov object to ua nodes via UA_Nodestore as custom interface.
 */

#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif

#include "demoTrafo.h"
#include "opcua.h"
#include "helpers.h"


static void demoTrafo_deleteNodestore(void *context){
}

static void demoTrafo_deleteNode(void * context, UA_Node *node){
	if (node){
		UA_Node_clear(node);
	}
	UA_free(node);
}

static void demoTrafo_releaseNode(void *context, const UA_Node *node){
	demoTrafo_deleteNode(context, (UA_Node*)node);
}

static void demoTrafo_iterate(void *context, UA_NodestoreVisitor visitor, void* visitorHandle){
}

static UA_Node * demoTrafo_newNode(void * context, UA_NodeClass nodeClass){
    return NULL;
}

static const UA_Node * demoTrafo_getNode(void * context, const UA_NodeId *nodeId){
	return NULL;
}

static UA_StatusCode demoTrafo_getNodeCopy(void *context, const UA_NodeId *nodeId, UA_Node ** nodeOut){
	UA_Node* node = (UA_Node*) demoTrafo_getNode(context, nodeId);
	if(node == NULL)
		return UA_STATUSCODE_BADNODEIDUNKNOWN;
	*nodeOut = node;
	return UA_STATUSCODE_GOOD;
}

static UA_StatusCode demoTrafo_removeNode(void *context, const UA_NodeId *nodeId){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

static UA_StatusCode demoTrafo_insertNode(void *context, UA_Node *node, UA_NodeId *parrentNode){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

static UA_StatusCode demoTrafo_replaceNode(void *context, UA_Node *node){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

UA_Nodestore*
demoTrafo_new(OV_INSTPTR_opcuaExample_customInterface context) {
	UA_Nodestore* nsi = (UA_Nodestore*)UA_malloc(sizeof(UA_Nodestore));
	if(nsi == NULL)
		return NULL;
    nsi->context =        	context;
    nsi->clear =  			demoTrafo_deleteNodestore;
    nsi->newNode =       	demoTrafo_newNode;
    nsi->deleteNode =    	demoTrafo_deleteNode;
    nsi->insertNode =       demoTrafo_insertNode;
    nsi->getNode =          demoTrafo_getNode;
    nsi->getNodeCopy =      demoTrafo_getNodeCopy;
    nsi->replaceNode =      demoTrafo_replaceNode;
    nsi->removeNode =       demoTrafo_removeNode;
    nsi->iterate =          demoTrafo_iterate;
    nsi->releaseNode =      demoTrafo_releaseNode;
	// Not needed, already handled by the nsSwitch
	nsi->getReferenceTypeId = NULL;
    return nsi;
}

void
demoTrafo_delete(UA_Nodestore * nodestoreInterface){
	nodestoreInterface->context = NULL;
	UA_free(nodestoreInterface);
}