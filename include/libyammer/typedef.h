#ifndef LIBYAMMER_TYPEDEF_H
#define LIBYAMMER_TYPEDEF_H

#include "../libyammer.h"

struct _YammerRequest;
struct _YammerResponse;

typedef struct _YammerRequest YammerRequest;
typedef struct _YammerResponse YammerResponse;

typedef enum
{
  YammerHttpGet = 1,
  YammerHttpPost
} YammerRequestType;

typedef void (*YammerRequestCompleteFunc)(YammerRequest* req,
                                          YammerResponse* res);

#endif
