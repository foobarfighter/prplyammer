#include "prplyammer/yammer_api.h"
#include <cometd.h>

#define YAPI_INVALID_RESPONSE  "Invalid response"
#define YAPI_CHAT_FEED_PATH "/api/v1/messages/private.json"

static void
yammer_impl_fetch_chat_feed_complete_cb (YammerRequest* req, YammerResponse* res)
{
  // YammerAccount* account;
  YammerApiCallbacks* cbs;
  gchar* token;
  
  cbs = req->userdata;

  if (yammer_api_validate_response (res, 200))
    return cbs->failure (YAPI_INVALID_RESPONSE, req);

  // if ((token = yammer_api_parse_feed (res->body)) == NULL)
  //   return cbs->failure ("couldn't parse feed info", req);

  // account = req->account;
  // account->oauth_token = token;
  // cbs->success (account, req);

  g_free(token);
}

gchar*
yammer_api_parse_feed (gchar* body)
{
  // JsonNode* node;
  // JsonObject *data_obj, *token_obj;
  // gchar* token_str;

  // node      = cometd_json_str2node (body);
  // data_obj  = json_node_get_object (node);
  // token_obj = json_object_get_object_member (data_obj, "access_token");
  // token_str = g_strdup (json_object_get_string_member (token_obj, "token"));

  // return token_str;
  return NULL;
}

gboolean
yammer_api_validate_response (YammerResponse* res, guint64 expected_code)
{
  g_return_val_if_fail(res != NULL, FALSE);
  g_return_val_if_fail(res->body != NULL, FALSE);
  g_return_val_if_fail(res->code != expected_code, FALSE);

  return TRUE;
}

void
yammer_api_fetch_chat_feed (YammerAccount* account,
                            YammerApiSuccessFunc success,
                            YammerApiFailureFunc failure)
{
  YammerRequest* req;
  YammerApiCallbacks* cbs;

  cbs = g_new0(YammerApiCallbacks, 1);
  cbs->success = success;
  cbs->failure = failure;

  // create a request 
  req = yammer_request_new(account,
                           YammerHttpGet,
                           yammer_impl_fetch_chat_feed_complete_cb,
                           YAPI_CHAT_FEED_PATH,
                           cbs);

  // // faking it
  // YammerResponse* fake_res = yammer_response_new();
  // fake_res->code = 200;
  // fake_res->body = "{ \"access_token\": { \"token\": \"4yji76RATKMNX4kH6tiDiQ\" } }";

  // req->response = fake_res;

  // yammer_impl_login_complete_cb(req, fake_res);
}
