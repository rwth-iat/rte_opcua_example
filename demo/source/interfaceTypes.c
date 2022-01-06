#ifndef OV_COMPILE_LIBRARY_opcuaExample
#define OV_COMPILE_LIBRARY_opcuaExample
#endif

#include "demo.h"
#include "ov_macros.h"
#include "trafoTypes.h"
#include "nodeset.h"
#include "helpers.h"
#include "nodeset_generated.h"


OV_DLLFNCEXPORT void demo_interfaceTypes_startup(
		OV_INSTPTR_ov_object 	pobj) {
	/*
	*   local variables
	*/
	OV_INSTPTR_demo_interfaceTypes pinst = Ov_StaticPtrCast(demo_interfaceTypes, pobj);

	/* do what the base class does first */
	ov_object_startup(pobj);

	/* do what */
	pinst->v_trafo = trafoTypes_new(pinst);

	return;
}

OV_DLLFNCEXPORT void demo_interfaceTypes_shutdown(
		OV_INSTPTR_ov_object 	pobj) {
	/*
	*   local variables
	*/
	OV_INSTPTR_demo_interfaceTypes pinst = Ov_StaticPtrCast(demo_interfaceTypes, pobj);

	/* do what */
	if (pinst->v_trafo) {
		trafoTypes_delete(pinst->v_trafo);
		pinst->v_trafo = NULL;
	}

	/* set the object's state to "shut down" */
	ov_object_shutdown(pobj);

	return;
}

OV_DLLFNCEXPORT OV_BOOL demo_interfaceTypes_checkNode(
		OV_INSTPTR_opcua_interface pInterface, OV_INSTPTR_ov_object pObj,
		OV_STRING virtualNodePath) {
	// Check if a functionchart is accessed via OPC UA and tell the OV switch (ovSwitch) to use our interface for the transformation
	if (Ov_CanCastTo(fb_functionchart, pObj))
	 	return TRUE;
	// Check if a virtual node variable is accessed (which is only "id" or "actimode" in our case)
	if (virtualNodePath != NULL)
		return TRUE;
	return FALSE;
}

UA_DataTypeArray customDataTypes = {NULL, UA_NODESET_COUNT, UA_NODESET};

OV_DLLFNCEXPORT OV_RESULT demo_interfaceTypes_load(OV_INSTPTR_opcua_interface pInterface, UA_Server* pServer) {
	// Load nodeset
	UA_StatusCode result = nodeset(pServer);

	// Load datatypes
	UA_ServerConfig* config = UA_Server_getConfig(pServer);
	config->customDataTypes = &customDataTypes;

	return result == UA_STATUSCODE_GOOD ? OV_ERR_OK : OV_ERR_GENERIC;
}