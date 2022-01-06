/* Generated from demo.bsd with script generate_datatypes.py
 * on host ALTAIR by user julian at 2022-01-06 04:06:00 */

#ifndef NODESET_GENERATED_HANDLING_H_
#define NODESET_GENERATED_HANDLING_H_

#include "nodeset_generated.h"

_UA_BEGIN_DECLS

#if defined(__GNUC__) && __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wmissing-field-initializers"
# pragma GCC diagnostic ignored "-Wmissing-braces"
#endif


/* Identifier */
static UA_INLINE void
UA_Identifier_init(UA_Identifier *p) {
    memset(p, 0, sizeof(UA_Identifier));
}

static UA_INLINE UA_Identifier *
UA_Identifier_new(void) {
    return (UA_Identifier*)UA_new(&UA_NODESET[UA_NODESET_IDENTIFIER]);
}

static UA_INLINE UA_StatusCode
UA_Identifier_copy(const UA_Identifier *src, UA_Identifier *dst) {
    return UA_copy(src, dst, &UA_NODESET[UA_NODESET_IDENTIFIER]);
}

UA_DEPRECATED static UA_INLINE void
UA_Identifier_deleteMembers(UA_Identifier *p) {
    UA_clear(p, &UA_NODESET[UA_NODESET_IDENTIFIER]);
}

static UA_INLINE void
UA_Identifier_clear(UA_Identifier *p) {
    UA_clear(p, &UA_NODESET[UA_NODESET_IDENTIFIER]);
}

static UA_INLINE void
UA_Identifier_delete(UA_Identifier *p) {
    UA_delete(p, &UA_NODESET[UA_NODESET_IDENTIFIER]);
}

/* Actimode */
static UA_INLINE void
UA_Actimode_init(UA_Actimode *p) {
    memset(p, 0, sizeof(UA_Actimode));
}

static UA_INLINE UA_Actimode *
UA_Actimode_new(void) {
    return (UA_Actimode*)UA_new(&UA_NODESET[UA_NODESET_ACTIMODE]);
}

static UA_INLINE UA_StatusCode
UA_Actimode_copy(const UA_Actimode *src, UA_Actimode *dst) {
    return UA_copy(src, dst, &UA_NODESET[UA_NODESET_ACTIMODE]);
}

UA_DEPRECATED static UA_INLINE void
UA_Actimode_deleteMembers(UA_Actimode *p) {
    UA_clear(p, &UA_NODESET[UA_NODESET_ACTIMODE]);
}

static UA_INLINE void
UA_Actimode_clear(UA_Actimode *p) {
    UA_clear(p, &UA_NODESET[UA_NODESET_ACTIMODE]);
}

static UA_INLINE void
UA_Actimode_delete(UA_Actimode *p) {
    UA_delete(p, &UA_NODESET[UA_NODESET_ACTIMODE]);
}

#if defined(__GNUC__) && __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
# pragma GCC diagnostic pop
#endif

_UA_END_DECLS

#endif /* NODESET_GENERATED_HANDLING_H_ */
