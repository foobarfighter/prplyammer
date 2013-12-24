#include "check_libyammer.h"

START_TEST (test_libyammer)
{
  fail_if(1);
}
END_TEST

Suite* make_yammer_suite (void)
{
  Suite *s = suite_create ("libyammer");

  /* Unit tests */
  TCase *tc_unit = tcase_create ("core");
  tcase_add_test (tc_unit, test_libyammer);
  suite_add_tcase (s, tc_unit);

  return s;
}

int
main (void)
{
  int number_failed;

  SRunner *sr = srunner_create (make_yammer_suite());
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

