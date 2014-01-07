#include "check_main.h"

static PurpleAccount* prpl_account = NULL;

static void setup_integration (void)
{
  init_libpurple();

  prpl_account = purple_account_new ("test_user", "yammer");
  purple_account_set_enabled (prpl_account, "test_runner", TRUE);
  purple_account_set_string (prpl_account, "oauth_token", env_test_token());
}

static void teardown_integration (void)
{
}

static gboolean kill_loop(gpointer user_data)
{
  GMainLoop* loop = user_data;
  g_main_loop_quit (loop);
}

static gboolean connect_account (gpointer user_data)
{
  PurpleAccount* account = user_data;
  purple_account_connect (account);
}

START_TEST (test_purple_account_connect_success)
{
  GMainLoop* loop = g_main_loop_new (NULL, FALSE);
  purple_timeout_add (100, connect_account, prpl_account);
  purple_timeout_add (3000, kill_loop, loop);
  g_main_loop_run (loop);
  fail_unless (0);
}
END_TEST

Suite* make_prplyammer_suite (void)
{
  Suite *s = suite_create ("prplyammer");

  if (is_env_loaded ())
  {
    TCase *tc_integration = tcase_create ("integration");
    tcase_add_checked_fixture (tc_integration, setup_integration, teardown_integration);
    tcase_add_test (tc_integration, test_purple_account_connect_success);
    suite_add_tcase (s, tc_integration);
  }

  return s;
}
