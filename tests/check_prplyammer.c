#include "check_main.h"

START_TEST (test_prplyammer)
{
  fail_if(1);
}
END_TEST

Suite* make_prplyammer_suite (void)
{
  Suite *s = suite_create ("prplyammer");

  /* Unit tests */
  TCase *tc_unit = tcase_create ("core");
  tcase_add_test (tc_unit, test_prplyammer);
  suite_add_tcase (s, tc_unit);

  return s;
}
