#include "prplyammer/yammer_account.h"

YammerAccount*
yammer_account_new (PurpleAccount* prpl_account)
{
  YammerAccount* account = g_new0(YammerAccount, 1);
  account->prpl_account = prpl_account;
  return account;
}
