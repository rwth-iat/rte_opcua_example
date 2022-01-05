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
#include "opcua.h"

/*
 * Nodestore API functions
 */
static const UA_Node * getNode(void * context, const UA_NodeId *nodeId){
	return NULL;
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
trafo_new(OV_INSTPTR_template_interface context) {
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