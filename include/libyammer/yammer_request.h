#ifndef LIBYAMMER_YAMMER_REQUEST_H
#define LIBYAMMER_YAMMER_REQUEST_H

#include "../libyammer.h"

struct _YammerRequest {
  GHashTable* headers;
  GHashTable* query;
  GHashTable* params;

  gpointer userdata;
  gchar* data;
  gchar* path;

  YammerRequestType type;
  YammerResponse* response;
  YammerRequestCompleteFunc on_complete;
};

YammerRequest* yammer_request_new (YammerRequestType type,
                                   const gchar* path,
                                   YammerRequestCompleteFunc complete_cb,
                                   gpointer user_data);

void yammer_request_destroy (YammerRequest* request);
void yammer_request_add_header (YammerRequest* request, const gchar* name, const gchar* value);
void yammer_request_set_data (YammerRequest* req, const gchar* data);

// yammer_request_build_json_post();
// yammer_request_add_header(request, name, value);
// yammer_request_add_param(request, name, value);
// yammer_request_add_query_param(request, name, value);
// yammer_request_add_account(request, account);
// yammer_request_execute(request);


#endif /* LIBYAMMER_YAMMER_REQUEST_H */
