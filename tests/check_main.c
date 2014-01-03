#include "check_main.h"

int
main (void)
{
  int number_failed;

  SRunner *sr = srunner_create (make_prplyammer_suite());
  srunner_add_suite (sr, make_yammer_response_suite());
  srunner_add_suite (sr, make_yammer_request_suite());

  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
