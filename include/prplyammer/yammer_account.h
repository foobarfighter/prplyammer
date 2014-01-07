#ifndef prplyammer_YAMMER_ACCOUNT_H
#define prplyammer_YAMMER_ACCOUNT_H

#include "../prplyammer.h"

struct _YammerAccount
{
  PurpleAccount* prpl_account;
  gchar* oauth_token;
};

YammerAccount* yammer_account_new (PurpleAccount* prpl_account);

#endif  /* prplyammer_YAMMER_ACCOUNT_H */
