#ifndef OV_COMPILE_LIBRARY_opcuaExample
#define OV_COMPILE_LIBRARY_opcuaExample
#endif

#include "opcuaExample.h"
#include "ov_macros.h"

#include "demoTrafo.h"


OV_DLLFNCEXPORT void opcuaExample_customInterface_startup(
		OV_INSTPTR_ov_object 	pobj) {
	/*
	*   local variables
	*/
	OV_INSTPTR_opcuaExample_customInterface pinst = Ov_StaticPtrCast(opcuaExample_customInterface, pobj);

	/* do what the base class does first */
	ov_object_startup(pobj);

	/* do what */
	pinst->v_trafo = demoTrafo_new(pinst);

	return;
}

OV_DLLFNCEXPORT void opcuaExample_customInterface_shutdown(
		OV_INSTPTR_ov_object 	pobj) {
	/*
	*   local variables
	*/
	OV_INSTPTR_opcuaExample_customInterface pinst = Ov_StaticPtrCast(opcuaExample_customInterface, pobj);

	/* do what */
	if (pinst->v_trafo) {
		demoTrafo_delete(pinst->v_trafo);
		pinst->v_trafo = NULL;
	}

	/* set the object's state to "shut down" */
	ov_object_shutdown(pobj);

	return;
}

OV_DLLFNCEXPORT OV_BOOL opcuaExample_customInterface_checkNode(
		OV_INSTPTR_opcua_interface pobj, OV_INSTPTR_ov_object pNode,
		OV_STRING virtualNodePath, void *context) {
	//OV_INSTPTR_opcuaExample_customInterface pinst = Ov_StaticPtrCast(opcuaExample_customInterface, pobj);
	// Check if custom interface trafo should be used
	if (FALSE) {
		return TRUE;
	}
	return FALSE;
}

OV_DLLFNCEXPORT OV_BOOL opcuaExample_customInterface_checkReference(
		OV_INSTPTR_opcua_interface pobj, OV_UINT applicationIndex,
		OV_INSTPTR_ov_object pNode, UA_AddReferencesItem * parentRef) {
	if (FALSE){
		return TRUE;
	}
	return FALSE;
}