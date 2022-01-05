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
		OV_INSTPTR_opcua_interface pInterface, OV_INSTPTR_ov_object pObj,
		OV_STRING virtualNodePath) {
	// Check if custom interface trafo should be used
	if (Ov_CanCastTo(fb_functionchart, pObj)) {
		return TRUE;
	}
	return FALSE;
}