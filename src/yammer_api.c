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

const gchar*
json_array_get_first_string_member (JsonNode* node)
{
  g_return_val_if_fail (JSON_NODE_HOLDS_ARRAY (node), NULL);

  JsonArray* arr = json_node_get_array (node);
  size_t len = json_array_get_length(arr);
  g_return_val_if_fail (len >= 1, NULL);

  return json_array_get_string_element (arr, 0);
}

gchar*
json_array_dup_first_string_member (JsonNode* node)
{
  return g_strdup (json_array_get_first_string_member(node));
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

static GList*
json_array_flatten_elements(JsonNode* n_array)
{
  GList *list = NULL, *arrs = NULL, *iarr = NULL;
  
  arrs = json_array_get_elements(json_node_get_array(n_array));

  for (iarr = arrs; iarr; iarr = g_list_next(iarr))
  {
    JsonArray* arr = json_node_get_array(iarr->data);
    list = g_list_concat(list, json_array_get_elements(arr));
  }

  return list;
}

GList*
yammer_api_read_messages (JsonNode* payload)
{
  GList* ymsgs = NULL;

  JsonNode* n_type = json_path_query ("$.data.type", payload, NULL);
  const gchar* type = json_array_get_first_string_member(n_type);

  g_return_val_if_fail(type != NULL, NULL);

  if (strcmp (type, "message") != 0)
    return NULL;

  JsonNode* n_messages = json_path_query ("$.data.data.messages", payload, NULL);
  JsonNode* n_references = json_path_query ("$.data.data.references", payload, NULL);

  GList *messages, *imessage, *references, *iref;
  
  messages = json_array_flatten_elements(n_messages);
  references = json_array_flatten_elements(n_references);

  for (imessage = messages; imessage; imessage = g_list_next(imessage))
  {
    YammerApiMessage* ymsg = g_new0(YammerApiMessage, 1);

    JsonObject* msg = json_node_get_object(imessage->data);

    gint64 msg_id = json_object_get_int_member(msg, "id");
    gint64 sender_id = json_object_get_int_member(msg, "sender_id");
    const gchar* sender_type = json_object_get_string_member(msg, "sender_type");

    for (iref = references; iref; iref = g_list_next(iref))
    {
      JsonObject* ref = json_node_get_object(iref->data);

      gint64 ref_id = json_object_get_int_member(ref, "id");
      const gchar* ref_type = json_object_get_string_member(ref, "type");
      const gchar* full_name = json_object_get_string_member(ref, "full_name");

      if (ref_id == sender_id && strcmp(ref_type, sender_type) == 0)
      {
        ymsg->id = msg_id;
        ymsg->sender_id = sender_id;
        ymsg->sender_full_name = g_strdup(full_name);
        ymsg->sender_type = g_strdup(sender_type);
        
        JsonObject* obody = json_object_get_object_member(msg, "body");
        ymsg->body = g_strdup(json_object_get_string_member(obody, "parsed"));

        ymsgs = g_list_append(ymsgs, ymsg);
        break;
      }
    }
  }

  return ymsgs;
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
