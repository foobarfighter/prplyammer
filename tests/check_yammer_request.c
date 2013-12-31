#include "check_main.h"

START_TEST (test_yammer_request_new)
{
  YammerRequest* req;

  req = yammer_request_new(YammerHttpPost, "/api/path", NULL, NULL);

  fail_unless(req->type == YammerHttpPost);
  ck_assert_str_eq("/api/path", req->path);

  yammer_request_destroy(req);
}
END_TEST

START_TEST (test_yammer_request_serialize)
{
  YammerRequest* req;

  gsize len = 4096;
  gchar buffer[len];

  req = yammer_request_new(YammerHttpPost, "/api/v1/path.json", NULL, NULL);

  yammer_request_serialize(req, buffer, len);

  ck_assert_str_eq(
    "POST /api/v1/path.json HTTP/1.0\r\n" \
    "Host: api.yammer.com\r\n" \
    "\r\n",
    buffer);

  yammer_request_add_header(req, "Content-Type", "application/json");
  yammer_request_serialize(req, buffer, len);
  ck_assert_str_eq(
    "POST /api/v1/path.json HTTP/1.0\r\n" \
    "Host: api.yammer.com\r\n" \
    "Content-Type: application/json\r\n" \
    "\r\n",
    buffer);

  yammer_request_destroy(req);
}
END_TEST

Suite* make_yammer_request_suite (void)
{
  Suite *s = suite_create ("yammer_request");

  /* Unit tests */
  TCase *tc_unit = tcase_create ("core");
  tcase_add_test (tc_unit, test_yammer_request_new);
  tcase_add_test (tc_unit, test_yammer_request_serialize);
  suite_add_tcase (s, tc_unit);

  return s;
}
