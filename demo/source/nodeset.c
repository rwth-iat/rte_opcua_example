/* WARNING: This is a generated file.
 * Any manual changes will be overwritten. */

#include "nodeset.h"


/* IdentifierType - ns=1;i=3 */

static UA_StatusCode function_nodeset_0_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_DataTypeAttributes attr = UA_DataTypeAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "IdentifierType");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
attr.description = UA_LOCALIZEDTEXT("", "Id struct to represent idH and idL of an ov object.");
#endif
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_DATATYPE,
UA_NODEID_NUMERIC(ns[1], 3LU),
UA_NODEID_NUMERIC(ns[0], 22LU),
UA_NODEID_NUMERIC(ns[0], 45LU),
UA_QUALIFIEDNAME(ns[1], "IdentifierType"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_DATATYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_nodeset_0_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[1], 3LU)
);
}

/* Default XML - ns=1;i=5 */

static UA_StatusCode function_nodeset_1_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_ObjectAttributes attr = UA_ObjectAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "Default XML");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECT,
UA_NODEID_NUMERIC(ns[1], 5LU),
UA_NODEID_NUMERIC(ns[0], 0LU),
UA_NODEID_NUMERIC(ns[0], 0LU),
UA_QUALIFIEDNAME(ns[0], "Default XML"),
UA_NODEID_NUMERIC(ns[0], 76LU),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[1], 5LU), UA_NODEID_NUMERIC(ns[0], 38LU), UA_EXPANDEDNODEID_NUMERIC(ns[1], 3LU), false);
return retVal;
}

static UA_StatusCode function_nodeset_1_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[1], 5LU)
);
}

/* Default Binary - ns=1;i=4 */

static UA_StatusCode function_nodeset_2_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_ObjectAttributes attr = UA_ObjectAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "Default Binary");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECT,
UA_NODEID_NUMERIC(ns[1], 4LU),
UA_NODEID_NUMERIC(ns[0], 0LU),
UA_NODEID_NUMERIC(ns[0], 0LU),
UA_QUALIFIEDNAME(ns[0], "Default Binary"),
UA_NODEID_NUMERIC(ns[0], 76LU),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[1], 4LU), UA_NODEID_NUMERIC(ns[0], 38LU), UA_EXPANDEDNODEID_NUMERIC(ns[1], 3LU), false);
return retVal;
}

static UA_StatusCode function_nodeset_2_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[1], 4LU)
);
}

/* ActimodeType - ns=1;i=2 */

static UA_StatusCode function_nodeset_3_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_DataTypeAttributes attr = UA_DataTypeAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "ActimodeType");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
attr.description = UA_LOCALIZEDTEXT("", "Enumeration data type for an actimode of a fb task (c.f. FB_AM_... in fb_namedef.h)");
#endif
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_DATATYPE,
UA_NODEID_NUMERIC(ns[1], 2LU),
UA_NODEID_NUMERIC(ns[0], 29LU),
UA_NODEID_NUMERIC(ns[0], 45LU),
UA_QUALIFIEDNAME(ns[1], "ActimodeType"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_DATATYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_nodeset_3_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[1], 2LU)
);
}

/* EnumValues - ns=1;i=8 */

static UA_StatusCode function_nodeset_4_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
attr.valueRank = 1;
attr.arrayDimensionsSize = 1;
UA_UInt32 arrayDimensions[1];
arrayDimensions[0] = 5;
attr.arrayDimensions = &arrayDimensions[0];
attr.dataType = UA_NODEID_NUMERIC(ns[0], 7594LU);
UA_EnumValueType variablenode_ns_1_i_8_variant_DataContents[5];

UA_init(&variablenode_ns_1_i_8_variant_DataContents[0], &UA_TYPES[UA_TYPES_ENUMVALUETYPE]);
variablenode_ns_1_i_8_variant_DataContents[0].value = (UA_Int64) 0;
variablenode_ns_1_i_8_variant_DataContents[0].displayName = UA_LOCALIZEDTEXT("", "OFF");

UA_init(&variablenode_ns_1_i_8_variant_DataContents[1], &UA_TYPES[UA_TYPES_ENUMVALUETYPE]);
variablenode_ns_1_i_8_variant_DataContents[1].value = (UA_Int64) 1;
variablenode_ns_1_i_8_variant_DataContents[1].displayName = UA_LOCALIZEDTEXT("", "ON");

UA_init(&variablenode_ns_1_i_8_variant_DataContents[2], &UA_TYPES[UA_TYPES_ENUMVALUETYPE]);
variablenode_ns_1_i_8_variant_DataContents[2].value = (UA_Int64) 2;
variablenode_ns_1_i_8_variant_DataContents[2].displayName = UA_LOCALIZEDTEXT("", "UNLINK");

UA_init(&variablenode_ns_1_i_8_variant_DataContents[3], &UA_TYPES[UA_TYPES_ENUMVALUETYPE]);
variablenode_ns_1_i_8_variant_DataContents[3].value = (UA_Int64) 3;
variablenode_ns_1_i_8_variant_DataContents[3].displayName = UA_LOCALIZEDTEXT("", "ONCE");

UA_init(&variablenode_ns_1_i_8_variant_DataContents[4], &UA_TYPES[UA_TYPES_ENUMVALUETYPE]);
variablenode_ns_1_i_8_variant_DataContents[4].value = (UA_Int64) 10;
variablenode_ns_1_i_8_variant_DataContents[4].displayName = UA_LOCALIZEDTEXT("", "CATCHUP");
UA_Variant_setArray(&attr.value, &variablenode_ns_1_i_8_variant_DataContents, (UA_Int32) 5, &UA_TYPES[UA_TYPES_ENUMVALUETYPE]);
attr.displayName = UA_LOCALIZEDTEXT("", "EnumValues");
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[1], 8LU),
UA_NODEID_NUMERIC(ns[1], 2LU),
UA_NODEID_NUMERIC(ns[0], 46LU),
UA_QUALIFIEDNAME(ns[0], "EnumValues"),
UA_NODEID_NUMERIC(ns[0], 68LU),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);





retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[1], 8LU), UA_NODEID_NUMERIC(ns[0], 37LU), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78LU), true);
return retVal;
}

static UA_StatusCode function_nodeset_4_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[1], 8LU)
);
}

/* Functionchart - ns=1;i=1 */

static UA_StatusCode function_nodeset_5_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_ObjectTypeAttributes attr = UA_ObjectTypeAttributes_default;
attr.displayName = UA_LOCALIZEDTEXT("", "Functionchart");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
attr.description = UA_LOCALIZEDTEXT("", "Type for the OPC UA representation of a ACPLT/FB functionchart");
#endif
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECTTYPE,
UA_NODEID_NUMERIC(ns[1], 1LU),
UA_NODEID_NUMERIC(ns[0], 58LU),
UA_NODEID_NUMERIC(ns[0], 45LU),
UA_QUALIFIEDNAME(ns[1], "Functionchart"),
 UA_NODEID_NULL,
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES],NULL, NULL);
return retVal;
}

static UA_StatusCode function_nodeset_5_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[1], 1LU)
);
}

/* actimode - ns=1;i=16 */

static UA_StatusCode function_nodeset_6_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
/* Value rank inherited */
attr.valueRank = -2;
attr.dataType = UA_NODEID_NUMERIC(ns[1], 2LU);
attr.displayName = UA_LOCALIZEDTEXT("", "actimode");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
attr.description = UA_LOCALIZEDTEXT("", "Transformed actimode from fb_task class, with value as enumeration.");
#endif
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[1], 16LU),
UA_NODEID_NUMERIC(ns[1], 1LU),
UA_NODEID_NUMERIC(ns[0], 46LU),
UA_QUALIFIEDNAME(ns[1], "actimode"),
UA_NODEID_NUMERIC(ns[0], 68LU),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[1], 16LU), UA_NODEID_NUMERIC(ns[0], 37LU), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78LU), true);
return retVal;
}

static UA_StatusCode function_nodeset_6_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[1], 16LU)
);
}

/* id - ns=1;i=15 */

static UA_StatusCode function_nodeset_7_begin(UA_Server *server, UA_UInt16* ns) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
UA_VariableAttributes attr = UA_VariableAttributes_default;
attr.minimumSamplingInterval = 0.000000;
attr.userAccessLevel = 1;
attr.accessLevel = 1;
/* Value rank inherited */
attr.valueRank = -2;
attr.dataType = UA_NODEID_NUMERIC(ns[1], 3LU);
attr.displayName = UA_LOCALIZEDTEXT("", "id");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
attr.description = UA_LOCALIZEDTEXT("", "Virtual id variable, which is a struct containing idH and idL of the functionchart.");
#endif
retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
UA_NODEID_NUMERIC(ns[1], 15LU),
UA_NODEID_NUMERIC(ns[1], 1LU),
UA_NODEID_NUMERIC(ns[0], 46LU),
UA_QUALIFIEDNAME(ns[1], "id"),
UA_NODEID_NUMERIC(ns[0], 68LU),
(const UA_NodeAttributes*)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES],NULL, NULL);
retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[1], 15LU), UA_NODEID_NUMERIC(ns[0], 37LU), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78LU), true);
return retVal;
}

static UA_StatusCode function_nodeset_7_finish(UA_Server *server, UA_UInt16* ns) {
return UA_Server_addNode_finish(server, 
UA_NODEID_NUMERIC(ns[1], 15LU)
);
}

UA_StatusCode nodeset(UA_Server *server) {
UA_StatusCode retVal = UA_STATUSCODE_GOOD;
/* Use namespace ids generated by the server */
UA_UInt16 ns[2];
ns[0] = UA_Server_addNamespace(server, "http://opcfoundation.org/UA/");
ns[1] = UA_Server_addNamespace(server, "http://github.com/acplt/rte_opcua_example/Nodeset/");

/* Load custom datatype definitions into the server */
if((retVal = function_nodeset_0_begin(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
if((retVal = function_nodeset_1_begin(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
if((retVal = function_nodeset_2_begin(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
if((retVal = function_nodeset_3_begin(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
if((retVal = function_nodeset_4_begin(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
if((retVal = function_nodeset_5_begin(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
if((retVal = function_nodeset_6_begin(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
if((retVal = function_nodeset_7_begin(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
if((retVal = function_nodeset_7_finish(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
if((retVal = function_nodeset_6_finish(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
if((retVal = function_nodeset_5_finish(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
if((retVal = function_nodeset_4_finish(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
if((retVal = function_nodeset_3_finish(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
if((retVal = function_nodeset_2_finish(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
if((retVal = function_nodeset_1_finish(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
if((retVal = function_nodeset_0_finish(server, ns)) != UA_STATUSCODE_GOOD) return retVal;
return retVal;
}
