#include "check_main.h"

static guint64 api_success_called = 0;
static guint64 api_failure_called = 0;

static void
api_success_cb (gpointer p_account, YammerRequest* req)
{
  api_success_called++;
}

static void
api_failure_cb (const gchar* msg, YammerRequest* req)
{
  api_failure_called++;
}

START_TEST (test_yammer_api_fetch_chat_feed_success)
{
  YammerAccount account;
  account.oauth_token = "test";

  yammer_api_fetch_chat_feed(&account, api_success_cb, api_failure_cb);

  // fail_unless(account->oauth_token != NULL);
  fail_unless(api_success_called == 1);
}
END_TEST

Suite* make_yammer_api_suite (void)
{
  Suite *s = suite_create ("yammer_api");

  TCase *tc_integration = tcase_create ("integration");
  tcase_add_test (tc_integration, test_yammer_api_fetch_chat_feed_success);
  suite_add_tcase (s, tc_integration);

  return s;
}
