#ifndef PRPLYAMMER_YAMMER_SESSION_H
#define PRPLYAMMER_YAMMER_SESSION_H

#include "../prplyammer.h"
#include <cometd.h>

struct _YammerSession
{
  cometd* cometd;
  const gchar* oauth_token;
};

YammerSession* yammer_session_new(const gchar* oauth_token);

#endif /* PRPLYAMMER_YAMMER_SESSION_H */
