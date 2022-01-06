/* Generated from demo.bsd with script generate_datatypes.py * on host ALTAIR by user julian at 2022-01-06 04:06:00 */

#ifndef NODESET_GENERATED_H_
#define NODESET_GENERATED_H_

#include "open62541.h"

_UA_BEGIN_DECLS


/**
 * Every type is assigned an index in an array containing the type descriptions.
 * These descriptions are used during type handling (copying, deletion,
 * binary encoding, ...). */
#define UA_NODESET_COUNT 2
extern UA_EXPORT const UA_DataType UA_NODESET[UA_NODESET_COUNT];

/**
 * Identifier
 * ^^
 */
typedef struct {
    UA_UInt32 idH;
    UA_UInt32 idL;
} UA_Identifier;

#define UA_NODESET_IDENTIFIER 0

/**
 * Actimode
 * ^^^^^^^^
 */
typedef enum {
    UA_ACTIMODE_ON = 0,
    UA_ACTIMODE_OFF = 1,
    __UA_ACTIMODE_FORCE32BIT = 0x7fffffff
} UA_Actimode;
UA_STATIC_ASSERT(sizeof(UA_Actimode) == sizeof(UA_Int32), enum_must_be_32bit);

#define UA_NODESET_ACTIMODE 1


_UA_END_DECLS

#endif /* NODESET_GENERATED_H_ */
