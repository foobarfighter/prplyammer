#include "libyammer/yammer_response.h"

YammerResponse*
yammer_response_parse(gchar* data, size_t len)
{
  return g_new0(YammerResponse, 1);
}
