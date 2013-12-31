#include "libyammer/yammer_request.h"
#include <string.h>

YammerRequest*
yammer_request_new (YammerRequestType type,
                    const gchar* path,
                    YammerRequestCompleteFunc complete_cb,
                    gpointer userdata)
{
  YammerRequest* req = g_new0(YammerRequest, 1);
  req->type = type;
  req->path = g_strdup(path);
  req->on_complete = complete_cb;
  req->userdata = userdata;
  return req;
}

void
yammer_request_destroy (YammerRequest* req)
{
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
yammer_request_set_data (YammerRequest* req, const gchar* data)
{
  req->data = g_strdup(data);
}

static void
yammer_impl_append_header(gpointer key, gpointer value, gpointer data)
{
  GString* request = (GString*) data;
  g_string_append_printf(request, "%s: %s\r\n", (gchar*) key, (gchar*) value);
}

void
yammer_request_serialize (YammerRequest* req, gchar* str, gsize len)
{
  GString* request = g_string_new(NULL);

  g_string_append_printf(request, "%s %s HTTP/1.0\r\n",
      req->type == YammerHttpPost ? "POST" : "GET",
      req->path);

  // Host should not be added as a header (not asserting that though)
  g_string_append(request, "Host: api.yammer.com\r\n");

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
  gsize copied = g_strlcpy(str, request->str, len);
  g_string_free(request, TRUE);

  if (copied >= len)
    purple_debug_info("yammer", "truncated request");
}
