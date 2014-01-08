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

struct _YammerApiMessage
{
  gint64 id;
  gint64 sender_id;
  gchar* sender_type;
  gchar* sender_full_name;
  gchar* body;
  time_t mtime;
};

void yammer_api_fetch_chat_feed (YammerAccount* account, YammerApiSuccessFunc success, YammerApiFailureFunc failure);
YammerApiFeed* yammer_api_parse_feed (gchar* body);
GList* yammer_api_read_messages(JsonNode* payload);
gboolean yammer_api_validate_response (YammerResponse* res, guint64 expected_code);

#endif  /* PRPLYAMMER_YAMMER_API_H */
