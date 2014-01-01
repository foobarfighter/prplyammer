#ifndef LIBYAMMER_YAMMER_ACCOUNT_H
#define LIBYAMMER_YAMMER_ACCOUNT_H

#include "../libyammer.h"

struct _YammerAccount
{
  PurpleAccount* prpl_account;
};

YammerAccount* yammer_account_new (PurpleAccount* prpl_account);

#endif  /* LIBYAMMER_YAMMER_ACCOUNT_H */
