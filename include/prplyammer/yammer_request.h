#ifndef prplyammer_YAMMER_REQUEST_H
#define prplyammer_YAMMER_REQUEST_H

#include "../prplyammer.h"

struct _YammerRequest {
  GHashTable* headers;
  GHashTable* query;
  GHashTable* params;

  gpointer userdata;
  gchar* data;
  gchar* path;
  GString* input_str;

  PurpleSslConnection* ssl_conn;

  YammerAccount* account;
  YammerRequestType type;
  YammerResponse* response;
  YammerRequestCompleteFunc on_complete;
};

YammerRequest* yammer_request_new (YammerAccount* account,
                                   YammerRequestType type,
                                   YammerRequestCompleteFunc complete_cb,
                                   const gchar* path,
                                   gpointer userdata);

void yammer_request_destroy (YammerRequest* request);
void yammer_request_add_header (YammerRequest* request, const gchar* name, const gchar* value);
void yammer_request_add_query_param(YammerRequest* request, const gchar* name, const gchar* value);
void yammer_request_set_data (YammerRequest* req, const gchar* data);
void yammer_request_execute (YammerRequest* req);
size_t yammer_request_serialize (YammerRequest* req, gchar* str, gsize len);

// yammer_request_build_json_post();
// yammer_request_add_query_param(request, name, value);
// yammer_request_add_account(request, account);


#endif /* prplyammer_YAMMER_REQUEST_H */
