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
  account.oauth_token = env_test_token();

  yammer_api_fetch_chat_feed(&account, api_success_cb, api_failure_cb);
  fail_unless(api_success_called == 1);
}
END_TEST

START_TEST (test_yammer_api_read_messages)
{
  JsonNode* node = json_from_fixture("message_payload");
  GList* messages = yammer_api_read_messages(node);
  ck_assert_int_eq(1, g_list_length(messages));

  YammerApiMessage* message = (YammerApiMessage*) g_list_nth_data(messages, 0);

  ck_assert_int_eq(111, message->id);
  ck_assert_int_eq(111, message->sender_id);
  ck_assert_str_eq("Bar Bar", message->sender_full_name);
  ck_assert_str_eq("user", message->sender_type);
  ck_assert_str_eq("hey now", message->body);
}
END_TEST

Suite* make_yammer_api_suite (void)
{
  Suite *s = suite_create ("yammer_api");

  TCase* tc_unit = tcase_create ("core");
  tcase_add_test (tc_unit, test_yammer_api_read_messages);
  suite_add_tcase (s, tc_unit);

  if (is_env_loaded())
  {
    TCase *tc_integration = tcase_create ("integration");
    tcase_add_test (tc_integration, test_yammer_api_fetch_chat_feed_success);
    suite_add_tcase (s, tc_integration);
  }

  return s;
}
