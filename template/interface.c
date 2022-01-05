#ifndef OV_COMPILE_LIBRARY_opcuaExample
#define OV_COMPILE_LIBRARY_opcuaExample
#endif

#include "demo.h"
#include "ov_macros.h"
#include "trafo.h"


OV_DLLFNCEXPORT void template_interface_startup(
		OV_INSTPTR_ov_object 	pobj) {
	/*
	*   local variables
	*/
	OV_INSTPTR_template_interface pinst = Ov_StaticPtrCast(template_interface, pobj);

	/* do what the base class does first */
	ov_object_startup(pobj);

	/* do what */
	pinst->v_trafo = trafo_new(pinst);

	return;
}

OV_DLLFNCEXPORT void template_interface_shutdown(
		OV_INSTPTR_ov_object 	pobj) {
	/*
	*   local variables
	*/
	OV_INSTPTR_template_interface pinst = Ov_StaticPtrCast(template_interface, pobj);

	/* do what */
	if (pinst->v_trafo) {
		trafo_delete(pinst->v_trafo);
		pinst->v_trafo = NULL;
	}

	/* set the object's state to "shut down" */
	ov_object_shutdown(pobj);

	return;
}

OV_DLLFNCEXPORT OV_BOOL template_interface_checkNode(
		OV_INSTPTR_opcua_interface pInterface, OV_INSTPTR_ov_object pObj,
		OV_STRING virtualNodePath) {
	// Check if a functionchart is accessed via OPC UA and tell the OV switch (ovSwitch) to use our interface for the transformation
	if (Ov_CanCastTo(fb_functionchart, pObj)) {
		return TRUE;
	}
	return FALSE;
}