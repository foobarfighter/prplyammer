#include "check_main.h"
#include <libpurple/savedstatuses.h>

static PurpleAccount* prpl_account = NULL;

static void setup_integration (void)
{
  init_libpurple();

  prpl_account = purple_account_new ("test_user", YAMMER_PLUGIN_ID);
  purple_account_set_password(prpl_account, "dummypassword");
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
  PurpleSavedStatus* status;
  purple_account_connect (account);

  return G_SOURCE_REMOVE;
}

static gboolean poll_session_status (gpointer user_data)
{
  gboolean* connected = user_data;

  if (prpl_account == NULL || prpl_account->gc == NULL || prpl_account->gc->proto_data == NULL)
    return G_SOURCE_CONTINUE;

  YammerSession* session = (YammerSession*) prpl_account->gc->proto_data;
  cometd* h = session->cometd;

  if  (h != NULL && h->conn != NULL)
    *connected = cometd_conn_is_state (h->conn, COMETD_CONNECTED);

  return *connected ? G_SOURCE_REMOVE : G_SOURCE_CONTINUE;
}

START_TEST (test_purple_account_connect_success)
{
  gboolean did_connect = FALSE;
  GMainLoop* loop = g_main_loop_new (NULL, FALSE);

  purple_timeout_add (100, connect_account, prpl_account);
  purple_timeout_add (100, poll_session_status, &did_connect);
  purple_timeout_add (3000, kill_loop, loop);
 
  g_main_loop_run (loop);

  fail_unless (did_connect);
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
