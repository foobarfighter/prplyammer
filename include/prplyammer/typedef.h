#ifndef prplyammer_TYPEDEF_H
#define prplyammer_TYPEDEF_H

#include "../prplyammer.h"

struct _YammerRequest;
struct _YammerResponse;
struct _YammerApiCallbacks;
struct _YammerApiFeed;
struct _YammerApiMessage;
struct _YammerSession;

typedef struct _YammerRequest  YammerRequest;
typedef struct _YammerResponse YammerResponse;
typedef struct _YammerAccount  YammerAccount;
typedef struct _YammerApiCallbacks  YammerApiCallbacks;
typedef struct _YammerApiFeed  YammerApiFeed;
typedef struct _YammerApiMessage YammerApiMessage;
typedef struct _YammerSession  YammerSession;

typedef enum
{
  YammerHttpGet = 1,
  YammerHttpPost
} YammerRequestType;

typedef void (*YammerRequestCompleteFunc)(YammerRequest* req,
                                          YammerResponse* res);
typedef void (*YammerApiSuccessFunc)(gpointer model, YammerRequest* req);
typedef void (*YammerApiFailureFunc)(const gchar* msg, YammerRequest* req);

#endif
