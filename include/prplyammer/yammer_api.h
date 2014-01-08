#ifndef PRPLYAMMER_YAMMER_API_H
#define PRPLYAMMER_YAMMER_API_H

#include "../prplyammer.h"

struct _YammerApiCallbacks
{
  YammerApiSuccessFunc success;
  YammerApiFailureFunc failure;
};

struct _YammerApiFeed
{
  gchar* realtime_uri;
  gchar* realtime_channel;
};

void yammer_api_fetch_chat_feed (YammerAccount* account, YammerApiSuccessFunc success, YammerApiFailureFunc failure);
YammerApiFeed* yammer_api_parse_feed (gchar* body);
gboolean yammer_api_validate_response (YammerResponse* res, guint64 expected_code);

#endif  /* PRPLYAMMER_YAMMER_API_H */
