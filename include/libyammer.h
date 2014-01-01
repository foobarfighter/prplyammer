#ifndef LIBYAMMER_H
#define LIBYAMMER_H

#define YAMMER_PLUGIN_ID           "prpl-foobarfighter-yaydium"
#define YAMMER_PLUGIN_NAME         "Yammer"
#define YAMMER_PLUGIN_SUMMARY      "Yammer Protocol Plugin"
#define YAMMER_PLUGIN_DESCRIPTION  "Yammer Protocol Plugin"
#define YAMMER_PLUGIN_VERSION      "0.1"
#define YAMMER_PLUGIN_AUTHOR       "Bob Remeika <bob.remeika@gmail.com>"
#define YAMMER_PLUGIN_HOMEPAGE     "http://yaydium.com"

#define YAMMER_MAX_REQUEST_SIZE     10240

#ifndef PURPLE_PLUGINS
# define PURPLE_PLUGINS
#endif

#include <glib.h>
#include <libpurple/plugin.h>
#include <libpurple/prpl.h>
#include <libpurple/proxy.h>
#include <libpurple/dnsquery.h>
#include <libpurple/version.h>

#include "libyammer/typedef.h"
#include "libyammer/yammer_request.h"
#include "libyammer/yammer_response.h"
#include "libyammer/yammer_account.h"

#endif  /* LIBYAMMER_H */
