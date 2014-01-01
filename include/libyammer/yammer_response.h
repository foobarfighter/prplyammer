#ifndef LIBYAMMER_YAMMER_RESPONSE_H
#define LIBYAMMER_YAMMER_RESPONSE_H

#include "../libyammer.h"

struct _YammerResponse
{
  char* body;
  size_t len;
  long code;
};

YammerResponse* yammer_response_parse(gchar* data, size_t len);

#endif /* LIBYAMMER_YAMMER_RESPONSE_H */
