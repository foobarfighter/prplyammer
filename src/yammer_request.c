#include "prplyammer/yammer_request.h"
#include <string.h>
#include <errno.h>

#define YAMMER_HOST "www.yammer.com"

static void yammer_impl_append_header (gpointer key, gpointer value, gpointer data);
static void yammer_impl_ssl_connect_cb (gpointer, PurpleSslConnection *, PurpleInputCondition);
static void yammer_impl_ssl_connect_error_cb (PurpleSslConnection *, PurpleSslErrorType, gpointer);
static void yammer_impl_readdata_cb (gpointer, PurpleSslConnection *, PurpleInputCondition);
static void yammer_impl_finish_request(YammerRequest* req, gboolean is_error);

YammerRequest*
yammer_request_new (YammerAccount* account,
                    YammerRequestType type,
                    YammerRequestCompleteFunc complete_cb,
                    const gchar* path,
                    gpointer userdata)
{
  YammerRequest* req = g_new0(YammerRequest, 1);
  
  req->account = account;
  req->type = type;
  req->on_complete = complete_cb;
  req->path = g_strdup(path);
  req->userdata = userdata;
  req->input_str = g_string_new(NULL);

  return req;
}

void
yammer_request_destroy (YammerRequest* req)
{
  g_string_free(req->input_str, TRUE);
  g_free(req->data);
  g_free(req->path);
  g_free(req);
}

void
yammer_request_add_header (YammerRequest* req, const gchar* name, const gchar* value)
{
  if (req->headers == NULL)
    req->headers = g_hash_table_new(g_str_hash, g_str_equal);

  g_hash_table_insert(req->headers, g_strdup(name), g_strdup(value));
}

void
yammer_request_add_query_param (YammerRequest* req, const gchar* name, const gchar* value)
{

}

void
yammer_request_set_data (YammerRequest* req, const gchar* data)
{
  req->data = g_strdup(data);
}

size_t
yammer_request_serialize (YammerRequest* req, gchar* str, gsize len)
{
  GString* request = g_string_new(NULL);

  g_string_append_printf(request, "%s %s HTTP/1.0\r\n",
      req->type == YammerHttpPost ? "POST" : "GET",
      req->path);

  // Host should not be added as a header (not asserting that though)
  g_string_append_printf(request, "Host: %s\r\n", YAMMER_HOST);

  // Append headers
  if (req->headers != NULL)
    g_hash_table_foreach(req->headers, yammer_impl_append_header, request);

  // Add content length
  if (req->data != NULL)
    g_string_append_printf(request, "Content-Length: %zu\r\n", strlen(req->data));

  // Append header separator
  g_string_append(request, "\r\n");

  // Add data
  if (req->data != NULL)
    g_string_append(request, req->data);

  // Copy and free the string
  gsize attempted = g_strlcpy(str, request->str, len);
  g_string_free(request, TRUE);

  if (attempted >= len)
    purple_debug_warning("yammer", "truncated request\n");

  return attempted;
}

void
yammer_request_execute (YammerRequest* req)
{
  PurpleSslConnection* ssl_conn;

  ssl_conn = purple_ssl_connect(req->account->prpl_account,
                                YAMMER_HOST, 443,
                                yammer_impl_ssl_connect_cb,
                                yammer_impl_ssl_connect_error_cb,
                                req);
  req->ssl_conn = ssl_conn;
}

void
yammer_impl_ssl_connect_cb (gpointer data, PurpleSslConnection* ssl_conn, PurpleInputCondition cond)
{
  gchar buffer[YAMMER_MAX_REQUEST_SIZE];
  size_t len, written;

  YammerRequest* request = data; 

  len = yammer_request_serialize(request, buffer, YAMMER_MAX_REQUEST_SIZE);
  written = purple_ssl_write(ssl_conn, buffer, len);

  purple_ssl_input_add(ssl_conn, yammer_impl_readdata_cb, request);
}

static void
yammer_impl_readdata_cb (gpointer data, PurpleSslConnection* gsc, PurpleInputCondition cond)
{
  gchar buf[4096];
  ssize_t len;

  YammerRequest* req = data;

  while((len = purple_ssl_read(gsc, buf, sizeof(buf) - 1)) > 0)
  {
    buf[len] = '\0';
    g_string_append(req->input_str, buf);
  }

  if (len == 0)
    yammer_impl_finish_request(req, FALSE);
  else if (len < 0 && errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
    yammer_impl_finish_request(req, TRUE);
}

void
yammer_impl_finish_request(YammerRequest* req, gboolean is_error)
{
  purple_ssl_close(req->ssl_conn);

  YammerResponse* res;

  if (is_error)
  {
    res = yammer_response_new();
    res->code = -1;
  } else {
    res = yammer_response_parse(req->input_str->str);
  }
  
  req->response = res;
  req->on_complete(req, res);
  yammer_request_destroy(req);
}

void
yammer_impl_ssl_connect_error_cb (PurpleSslConnection* ssl_conn, PurpleSslErrorType type, gpointer data)
{
  yammer_impl_finish_request(data, TRUE);
}

void
yammer_impl_append_header(gpointer key, gpointer value, gpointer data)
{
  GString* request = (GString*) data;
  g_string_append_printf(request, "%s: %s\r\n", (gchar*) key, (gchar*) value);
}
