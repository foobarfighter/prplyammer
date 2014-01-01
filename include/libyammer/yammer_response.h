#ifndef LIBYAMMER_YAMMER_RESPONSE_H
#define LIBYAMMER_YAMMER_RESPONSE_H

#include "../libyammer.h"

struct _YammerResponse
{
  size_t len;
  gint64 code;
  char*  body;
};

YammerResponse* yammer_response_parse(gchar* data);

#endif /* LIBYAMMER_YAMMER_RESPONSE_H */
