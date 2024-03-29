#ifndef PRPLYAMMER_YAMMER_RESPONSE_H
#define PRPLYAMMER_YAMMER_RESPONSE_H

#include "../prplyammer.h"

struct _YammerResponse
{
  size_t len;
  gint64 code;
  char*  body;
};

YammerResponse* yammer_response_new(void);
YammerResponse* yammer_response_parse(gchar* data);

#endif /* PRPLYAMMER_YAMMER_RESPONSE_H */
