#include "prplyammer/yammer_session.h"

YammerSession*
yammer_session_new(const gchar* oauth_token)
{
  YammerSession* session = g_new0(YammerSession, 1);
  session->oauth_token = oauth_token;
  return session;
}
