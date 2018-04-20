#include <Condition.h>
#include <Fulfillment.h>
#include "cJSON.h"
#include "asn/asn_application.h"
#include "cryptoconditions.h"

#ifndef INTERNAL_H
#define INTERNAL_H


#ifdef __cplusplus
extern "C" {
#endif


#define BUF_SIZE 1024 * 1024

/*
 * Condition Type
 */
typedef struct CCType {
    int typeId;
    char name[100];
    Condition_PR asnType;
    int (*visitChildren)(CC *cond, CCVisitor visitor);
    unsigned char *(*fingerprint)(const CC *cond);
    unsigned long (*getCost)(const CC *cond);
    uint32_t (*getSubtypes)(const  CC *cond);
    CC *(*fromJSON)(const cJSON *params, char *err);
    void (*toJSON)(const CC *cond, cJSON *params);
    CC *(*fromFulfillment)(const Fulfillment_t *ffill);
    Fulfillment_t *(*toFulfillment)(const CC *cond);
    int (*isFulfilled)(const CC *cond);
    void (*free)(struct CC *cond);
} CCType;


/*
 * Globals
 */
extern struct CCType *CCTypeRegistry[32];
extern int CCTypeRegistryLength;


/*
 * Internal API
 */
uint32_t fromAsnSubtypes(ConditionTypes_t types);
CC *mkAnon(const Condition_t *asnCond);
void asnCondition(const CC *cond, Condition_t *asn);
Condition_t *asnConditionNew(const CC *cond);
Fulfillment_t *asnFulfillmentNew(const CC *cond);
struct CC *fulfillmentToCC(Fulfillment_t *ffill);
struct CCType *getTypeByAsnEnum(Condition_PR present);


/*
 * Utility functions
 */
unsigned char *base64_encode(const unsigned char *data, size_t input_length);
unsigned char *base64_decode(const unsigned char *data_, size_t *output_length);
unsigned char *hashFingerprintContents(asn_TYPE_descriptor_t *asnType, void *fp);
void dumpStr(unsigned char *str, size_t len);
int checkString(const cJSON *value, char *key, char *err);
int checkDecodeBase64(const cJSON *value, char *key, char *err, unsigned char **data, size_t *size);
int jsonGetBase64(const cJSON *params, char *key, char *err, unsigned char **data, size_t *size);
int jsonGetBase64Optional(const cJSON *params, char *key, char *err, unsigned char **data, size_t *size);
void jsonAddBase64(cJSON *params, char *key, unsigned char *bin, size_t size);
char* cc_hex_encode(const uint8_t *bin, size_t len);
uint8_t* cc_hex_decode(const char* hex);
extern int checkDecodeHex(const cJSON *params, char *key, char *err, uint8_t **data, size_t *size);
extern int jsonGetHex(const cJSON *params, char *key, char *err, unsigned char **data, size_t *size);
void jsonAddHex(cJSON *params, char *key, uint8_t *bin, size_t size);
int jsonGetHexOptional(const cJSON *params, char *key, char *err, unsigned char **data, size_t *size);


#ifdef __cplusplus
}
#endif

#endif  /* INTERNAL_H */
