/* Generated from demo.bsd with script generate_datatypes.py
 * on host ALTAIR by user julian at 2022-01-06 04:06:00 */

#include "nodeset_generated.h"

/* Identifier */
static UA_DataTypeMember Identifier_members[2] = {
{
    UA_TYPES_UINT32, /* .memberTypeIndex */
    0, /* .padding */
    true, /* .namespaceZero */
    false, /* .isArray */
    false  /* .isOptional */
    UA_TYPENAME("idH") /* .memberName */
},
{
    UA_TYPES_UINT32, /* .memberTypeIndex */
    offsetof(UA_Identifier, idL) - offsetof(UA_Identifier, idH) - sizeof(UA_UInt32), /* .padding */
    true, /* .namespaceZero */
    false, /* .isArray */
    false  /* .isOptional */
    UA_TYPENAME("idL") /* .memberName */
},};

/* Actimode */
#define Actimode_members NULL
const UA_DataType UA_NODESET[UA_NODESET_COUNT] = {
/* Identifier */
{
    {2, UA_NODEIDTYPE_NUMERIC, {3002LU}}, /* .typeId */
    {2, UA_NODEIDTYPE_NUMERIC, {5001LU}}, /* .binaryEncodingId */
    sizeof(UA_Identifier), /* .memSize */
    UA_NODESET_IDENTIFIER, /* .typeIndex */
    UA_DATATYPEKIND_STRUCTURE, /* .typeKind */
    false, /* .pointerFree */
    false, /* .overlayable */
    2, /* .membersSize */
    Identifier_members  /* .members */
    UA_TYPENAME("Identifier") /* .typeName */
},
/* Actimode */
{
    {2, UA_NODEIDTYPE_NUMERIC, {2LU}}, /* .typeId */
    {2, UA_NODEIDTYPE_NUMERIC, {0}}, /* .binaryEncodingId */
    sizeof(UA_Actimode), /* .memSize */
    UA_TYPES_INT32, /* .typeIndex */
    UA_DATATYPEKIND_ENUM, /* .typeKind */
    true, /* .pointerFree */
    UA_BINARY_OVERLAYABLE_INTEGER, /* .overlayable */
    0, /* .membersSize */
    Actimode_members  /* .members */
    UA_TYPENAME("Actimode") /* .typeName */
},
};

