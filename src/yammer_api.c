#include "prplyammer/yammer_api.h"
#include <cometd.h>

#define YAPI_INVALID_RESPONSE  "Invalid response"
#define YAPI_CHAT_FEED_PATH "/api/v1/messages/private.json"

static void
yammer_impl_feed_destroy(YammerApiFeed* feed)
{
  g_free(feed->realtime_channel);
  g_free(feed->realtime_uri);
  g_free(feed);
}

static void
yammer_impl_fetch_chat_feed_complete_cb (YammerRequest* req, YammerResponse* res)
{
  YammerApiCallbacks* cbs = req->userdata;
  YammerApiFeed* feed;
  
  if (!yammer_api_validate_response (res, 200))
    return cbs->failure (YAPI_INVALID_RESPONSE, req);

  if ((feed = yammer_api_parse_feed (res->body)) == NULL)
    return cbs->failure ("couldn't parse feed info", req);

  cbs->success (feed, req);

  yammer_impl_feed_destroy(feed);
}

gchar*
json_array_dup_first_string_member (JsonNode* node)
{
  g_return_val_if_fail (JSON_NODE_HOLDS_ARRAY (node), NULL);

  JsonArray* arr = json_node_get_array (node);
  size_t len = json_array_get_length(arr);
  g_return_val_if_fail (len >= 1, NULL);

  return g_strdup (json_array_get_string_element (arr, 0));
}

YammerApiFeed*
yammer_api_parse_feed (gchar* body)
{
  JsonNode* node = cometd_json_str2node(body);
  g_return_val_if_fail(node != NULL, NULL);

  YammerApiFeed* feed = g_new0(YammerApiFeed, 1);

  JsonNode* n_channel_id = json_path_query("$.meta.realtime.channel_id", node, NULL);
  JsonNode* n_uri = json_path_query("$.meta.realtime.uri", node, NULL);

  feed->realtime_channel = json_array_dup_first_string_member(n_channel_id);
  feed->realtime_uri = json_array_dup_first_string_member(n_uri);

  return feed;
}

gboolean
yammer_api_validate_response (YammerResponse* res, guint64 expected_code)
{
  g_return_val_if_fail(res != NULL, FALSE);
  g_return_val_if_fail(res->body != NULL, FALSE);
  g_return_val_if_fail(res->code == expected_code, FALSE);

  return TRUE;
}

void
yammer_api_fetch_chat_feed (YammerAccount* account,
                            YammerApiSuccessFunc success,
                            YammerApiFailureFunc failure)
{
  YammerRequest* req;
  YammerApiCallbacks* cbs;

  gchar buffer[4096] = {'\0'};
  sprintf(buffer, "Bearer %s", account->oauth_token);

  cbs = g_new0(YammerApiCallbacks, 1);
  cbs->success = success;
  cbs->failure = failure;

  // create a request 
  req = yammer_request_new(account,
                           YammerHttpGet,
                           yammer_impl_fetch_chat_feed_complete_cb,
                           YAPI_CHAT_FEED_PATH,
                           cbs);

  yammer_request_add_header(req, "Authorization", buffer);
  yammer_request_execute(req);
}
