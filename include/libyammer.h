#ifndef LIBYAMMER_H
#define LIBYAMMER_H

#define YAMMER_PLUGIN_ID           "prpl-foobarfighter-yammerim"
#define YAMMER_PLUGIN_NAME         "Yammer"
#define YAMMER_PLUGIN_SUMMARY      "Yammer Protocol Plugin"
#define YAMMER_PLUGIN_DESCRIPTION  "Yammer Protocol Plugin"
#define YAMMER_PLUGIN_VERSION      "0.1"
#define YAMMER_PLUGIN_AUTHOR       "Bob Remeika <bob.remeika@gmail.com>"
#define YAMMER_PLUGIN_HOMEPAGE     "http://yaydium.com"

#ifndef PURPLE_PLUGINS
# define PURPLE_PLUGINS
#endif

#include <glib.h>
#include <libpurple/plugin.h>
#include <libpurple/prpl.h>
#include <libpurple/version.h>

#endif  /* LIBYAMMER_H */