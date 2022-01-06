/* Generated from demo.bsd with script generate_datatypes.py
 * on host ALTAIR by user julian at 2022-01-06 04:06:00 */

#ifndef NODESET_GENERATED_ENCODING_BINARY_H_
#define NODESET_GENERATED_ENCODING_BINARY_H_

# include "open62541.h"

/* Id */
static UA_INLINE size_t
UA_Id_calcSizeBinary(const UA_Id *src) {
    return UA_calcSizeBinary(src, &UA_NODESET[UA_NODESET_IDENTIFIER]);
}
static UA_INLINE UA_StatusCode
UA_Id_encodeBinary(const UA_Id *src, UA_Byte **bufPos, const UA_Byte *bufEnd) {
    return UA_encodeBinary(src, &UA_NODESET[UA_NODESET_IDENTIFIER], bufPos, &bufEnd, NULL, NULL);
}
static UA_INLINE UA_StatusCode
UA_Id_decodeBinary(const UA_ByteString *src, size_t *offset, UA_Id *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_NODESET[UA_NODESET_IDENTIFIER], NULL);
}

/* Actimode */
static UA_INLINE size_t
UA_Actimode_calcSizeBinary(const UA_Actimode *src) {
    return UA_calcSizeBinary(src, &UA_NODESET[UA_NODESET_ACTIMODE]);
}
static UA_INLINE UA_StatusCode
UA_Actimode_encodeBinary(const UA_Actimode *src, UA_Byte **bufPos, const UA_Byte *bufEnd) {
    return UA_encodeBinary(src, &UA_NODESET[UA_NODESET_ACTIMODE], bufPos, &bufEnd, NULL, NULL);
}
static UA_INLINE UA_StatusCode
UA_Actimode_decodeBinary(const UA_ByteString *src, size_t *offset, UA_Actimode *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_NODESET[UA_NODESET_ACTIMODE], NULL);
}

#endif /* NODESET_GENERATED_ENCODING_BINARY_H_ */
