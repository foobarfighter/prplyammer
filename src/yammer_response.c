#include "libyammer/yammer_response.h"

static void yammer_impl_parse_header(YammerResponse* res, gchar* header);

YammerResponse*
yammer_response_parse(gchar* data)
{
  YammerResponse* res = g_new0(YammerResponse, 1);
  gchar** parts = g_strsplit(data, "\r\n\r\n", 2);

  // if (g_strv_length(parts) < 2)
    // return yammer_impl_fill_bad_response(res);

  yammer_impl_parse_header(res, parts[0]);
  res->body = parts[1];

  return res;
}

void
yammer_impl_parse_header (YammerResponse* res, gchar* header)
{
  gchar **header_parts, **status_parts;

  header_parts = g_strsplit(header, "\r\n", 0);
  status_parts = g_strsplit(header_parts[0], " ", 3);

  res->code = g_ascii_strtoll(status_parts[1], NULL, 10);
}

// yammer_response_valid
