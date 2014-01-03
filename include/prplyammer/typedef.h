#ifndef prplyammer_TYPEDEF_H
#define prplyammer_TYPEDEF_H

#include "../prplyammer.h"

struct _YammerRequest;
struct _YammerResponse;

typedef struct _YammerRequest YammerRequest;
typedef struct _YammerResponse YammerResponse;
typedef struct _YammerAccount YammerAccount;

typedef enum
{
  YammerHttpGet = 1,
  YammerHttpPost
} YammerRequestType;

typedef void (*YammerRequestCompleteFunc)(YammerRequest* req,
                                          YammerResponse* res);

#endif
