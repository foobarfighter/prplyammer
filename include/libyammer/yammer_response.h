#ifndef LIBYAMMER_YAMMER_RESPONSE_H
#define LIBYAMMER_YAMMER_RESPONSE_H

#include "../libyammer.h"

struct _YammerResponse
{
  char* body;
  size_t len;
  long code;
};

#endif /* LIBYAMMER_YAMMER_RESPONSE_H */
