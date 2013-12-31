#include "check_main.h"

START_TEST (test_libyammer)
{
  fail_if(1);
}
END_TEST

Suite* make_libyammer_suite (void)
{
  Suite *s = suite_create ("libyammer");

  /* Unit tests */
  TCase *tc_unit = tcase_create ("core");
  tcase_add_test (tc_unit, test_libyammer);
  suite_add_tcase (s, tc_unit);

  return s;
}
