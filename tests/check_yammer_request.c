#include "check_main.h"

static gchar* client_id = NULL;
static gchar* client_secret = NULL;
static gchar* test_token = NULL;
static YammerAccount* account = NULL;

static void setup_integration (void)
{
  init_libpurple();

  PurpleAccount* prpl_account = purple_account_new("test_user", "yammer");
  purple_account_set_enabled(prpl_account, "test_runner", TRUE);
  account = yammer_account_new(prpl_account);
}

static void teardown_integration (void)
{

}

START_TEST (test_yammer_request_new)
{
  YammerRequest* req;

  req = yammer_request_new(NULL, YammerHttpPost, NULL, "/api/path", NULL);

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

  req = yammer_request_new(NULL, YammerHttpPost, NULL, "/api/v1/path.json", NULL);

  yammer_request_serialize(req, buffer, len);

  ck_assert_str_eq(
    "POST /api/v1/path.json HTTP/1.0\r\n" \
    "Host: www.yammer.com\r\n" \
    "\r\n",
    buffer);

  yammer_request_add_header(req, "Content-Type", "application/json");
  yammer_request_serialize(req, buffer, len);
  ck_assert_str_eq(
    "POST /api/v1/path.json HTTP/1.0\r\n" \
    "Host: www.yammer.com\r\n" \
    "Content-Type: application/json\r\n" \
    "\r\n",
    buffer);

  yammer_request_set_data(req, "[1]");
  yammer_request_serialize(req, buffer, len);
  ck_assert_str_eq(
    "POST /api/v1/path.json HTTP/1.0\r\n" \
    "Host: www.yammer.com\r\n" \
    "Content-Type: application/json\r\n" \
    "Content-Length: 3\r\n" \
    "\r\n" \
    "[1]",
    buffer);

  yammer_request_destroy(req);
}
END_TEST

static gchar* log_data = NULL;

static void
log_cb (YammerRequest* req, YammerResponse* res)
{
  log_data = res->body;

  printf("body: %s\n", res->body);
}

START_TEST (test_yammer_request_integration)
{
  GMainLoop *loop = g_main_loop_new(NULL, FALSE);

  YammerRequest* req;
  gchar buffer[4096] = {'\0'};

  sprintf(buffer, "Bearer %s", test_token);

  req = yammer_request_new(account, YammerHttpGet, log_cb, "/api/v1/messages.json", NULL);
  yammer_request_add_header(req, "Authorization", buffer);
  yammer_request_execute(req);

  g_main_loop_run(loop);

  fail_if (log_data == NULL);

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

  /* Integration */
  client_id = getenv("CLIENT_ID");
  client_secret = getenv("CLIENT_SECRET");
  test_token = getenv("TEST_TOKEN");

  if (client_id != NULL && client_secret != NULL && test_token != NULL)
  {
    TCase *tc_integration = tcase_create ("integration");
    tcase_add_checked_fixture (tc_integration, setup_integration, teardown_integration);
    tcase_add_test (tc_integration, test_yammer_request_integration);
    suite_add_tcase (s, tc_integration);
  } else {
    printf("\nWARNING: Not running yammer_request::integration test cases " \
           "because CLIENT_ID, CLIENT_SECRET and/or TEST_TOKEN were not " \
           "provided\n\n");
  }

  return s;
}
