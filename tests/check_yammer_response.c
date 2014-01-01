#include "check_main.h"

static gchar* good =
  "HTTP/1.1 200 OK\r\n" \
  "Content-Type: application/json\r\n" \
  "Content-Length: 3\r\n" \
  "\r\n" \
  "[1]";

START_TEST (test_yammer_response_parse)
{
  YammerResponse* res = yammer_response_parse(good);
  ck_assert_int_eq(200, res->code);
  ck_assert_str_eq("[1]", res->body);
}
END_TEST

Suite* make_yammer_response_suite (void)
{
  Suite *s = suite_create ("yammer_response");

  /* Unit tests */
  TCase *tc_unit = tcase_create ("unit");
  tcase_add_test (tc_unit, test_yammer_response_parse);
  suite_add_tcase (s, tc_unit);

  return s;
}
