#include "prplyammer.h"
#include <stdio.h>
#include <cometd.h>
#include <cometd/exts/yammer.h>
#include <cometd/exts/logger.h>

static YammerAccount* g_account = NULL;

static gboolean
plugin_load(PurplePlugin *plugin)
{
  return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
  return TRUE;
}

static const char *
yammer_list_icon(PurpleAccount *account, PurpleBuddy *buddy)
{
  return "yammer";
}

static int
yammer_handle_message(const cometd* h, JsonNode* message)
{
  GList* messages = yammer_api_read_messages (message);

  GList* imessage;
  for (imessage = messages; imessage; imessage = g_list_next (imessage))
  {
    YammerApiMessage* msg = (YammerApiMessage*) imessage->data;

    serv_got_im (g_account->prpl_account->gc,
                 msg->sender_full_name,
                 msg->body,
                 PURPLE_MESSAGE_RECV,
                 msg->mtime);
  }

  // TODO: Should we use purple_timeout_add here in a cheap attempt
  //       to bypass libpurple's thread safety issues?
  return COMETD_SUCCESS;
}

static void
yammer_connect_realtime(YammerAccount* account, YammerApiFeed* feed)
{
  YammerSession* session;
  PurpleConnection* gc;
  cometd* cometd;
  gchar primary[512], secondary[512];

  gc = purple_account_get_connection (account->prpl_account);
  purple_connection_update_progress(gc, "Connecting to realtime", 1, 3);

  session = (YammerSession*) gc->proto_data;

  cometd = cometd_new ();
  cometd_configure (cometd, COMETDOPT_URL, feed->realtime_uri);
  cometd_ext_add (&cometd->exts, cometd_ext_yammer_new(account->oauth_token));
  cometd_ext_add (&cometd->exts, cometd_ext_logger_new());

  cometd_connect (cometd);

  sprintf (primary, "/feeds/%s/primary", feed->realtime_channel);
  sprintf (secondary, "/feeds/%s/secondary", feed->realtime_channel);

  g_account = account; // FIXME: cometd_subscribe needs to send along userdata
  cometd_subscribe (cometd, primary, yammer_handle_message);
  cometd_subscribe (cometd, secondary, yammer_handle_message);
  
  session->cometd = cometd;

  // // FIXME: libpurple is not thread safe
  cometd_listen_async (cometd);

  purple_connection_set_state(gc, PURPLE_CONNECTED);
  purple_connection_update_progress(gc, "Connected", 2, 3);
}

static void
yammer_chat_feed_success_cb (gpointer model, YammerRequest* req)
{
  yammer_connect_realtime (req->account, (YammerApiFeed*) model);
}

static void
yammer_chat_feed_failure_cb (const gchar* msg, YammerRequest* req)
{
  PurpleConnection* gc;

  gc = purple_account_get_connection (req->account->prpl_account);
  purple_connection_set_state(gc, PURPLE_CONNECTED);
}

static void
yammer_connect(YammerAccount* account)
{
  yammer_api_fetch_chat_feed (account,
                              yammer_chat_feed_success_cb,
                              yammer_chat_feed_failure_cb);
}

static void
yammer_login(PurpleAccount *account)
{
  PurpleConnection* gc;
  YammerAccount* yammer;
  YammerSession* session;
  gchar* token;

  yammer = yammer_account_new (account);
  token = g_strdup(purple_account_get_string (account, "oauth_token", ""));

  yammer->oauth_token = token;

  session = yammer_session_new (token);
  gc = purple_account_get_connection (yammer->prpl_account);
  gc->proto_data = session;

  purple_connection_set_state(gc, PURPLE_CONNECTING);
  purple_connection_update_progress(gc, "Connecting", 0, 3);

  yammer_connect (yammer);
}

static void
yammer_close(PurpleConnection *gc)
{
}

static PurplePluginProtocolInfo prpl_info = {
  /* options */
  OPT_PROTO_MAIL_CHECK,

  NULL,                   /* user_splits */
  NULL,                   /* protocol_options */
  // NO_BUDDY_ICONS,          /* icon_spec */
  {"jpg", 0, 0, 50, 50, -1, PURPLE_ICON_SCALE_SEND}, /* icon_spec */
  yammer_list_icon,       /* list_icon */
  NULL,                   /* list_emblems */
  NULL,                   /* status_text */
  NULL,                   /* tooltip_text */
  NULL,                   /* status_types */
  NULL,                   /* blist_node_menu */
  NULL,                   /* chat_info */
  NULL,                   /* chat_info_defaults */
  yammer_login,           /* login */
  yammer_close,           /* close */
  NULL,                   /* send_im */
  NULL,                   /* set_info */
  NULL,                   /* send_typing */
  NULL,                   /* get_info */
  NULL,                   /* set_status */
  NULL,                   /* set_idle */
  NULL,                   /* change_passwd */
  NULL,                   /* add_buddy */
  NULL,                   /* add_buddies */
  NULL,                   /* remove_buddy */
  NULL,                   /* remove_buddies */
  NULL,                   /* add_permit */
  NULL,                   /* add_deny */
  NULL,                   /* rem_permit */
  NULL,                   /* rem_deny */
  NULL,                   /* set_permit_deny */
  NULL,                   /* join_chat */
  NULL,                   /* reject chat invite */
  NULL,                   /* get_chat_name */
  NULL,                   /* chat_invite */
  NULL,                   /* chat_leave */
  NULL,                   /* chat_whisper */
  NULL,                   /* chat_send */
  NULL,                   /* keepalive */
  NULL,                   /* register_user */
  NULL,                   /* get_cb_info */
  NULL,                   /* get_cb_away */
  NULL,                   /* alias_buddy */
  NULL,                   /* group_buddy */
  NULL,                   /* rename_group */
  NULL,                   /* buddy_free */
  NULL,                   /* convo_closed */
  NULL,                   /* normalize */
  NULL,                   /* set_buddy_icon */
  NULL,                   /* remove_group */
  NULL,                   /* get_cb_real_name */
  NULL,                   /* set_chat_topic */
  NULL,                   /* find_blist_chat */
  NULL,                   /* roomlist_get_list */
  NULL,                   /* roomlist_cancel */
  NULL,                   /* roomlist_expand_category */
  NULL,                   /* can_receive_file */
  NULL,                   /* send_file */
  NULL,                   /* new_xfer */
  NULL,                   /* offline_message */
  NULL,                   /* whiteboard_prpl_ops */
  NULL,                   /* send_raw */
  NULL,                   /* roomlist_room_serialize */
  NULL,                   /* unregister_user */
  NULL,                   /* send_attention */
  NULL,                   /* attention_types */
#if PURPLE_MAJOR_VERSION >= 2 && PURPLE_MINOR_VERSION >= 5
  sizeof(PurplePluginProtocolInfo), /* struct_size */
  NULL,                   /* get_account_text_table */
#else   
  (gpointer) sizeof(PurplePluginProtocolInfo)
#endif
};

static PurplePluginInfo info = {
    PURPLE_PLUGIN_MAGIC,      /* magic */
    PURPLE_MAJOR_VERSION,     /* major version */
    PURPLE_MINOR_VERSION,     /* minor version */
    PURPLE_PLUGIN_PROTOCOL,   /* plugin type */
    NULL,                     /* ui requirement */
    0,                        /* flags */
    NULL,                     /* plugin dependencies */
    PURPLE_PRIORITY_DEFAULT,  /* priority */
    YAMMER_PLUGIN_ID,         /* plugin id */
    YAMMER_PLUGIN_NAME,       /* plugin name */
    YAMMER_PLUGIN_VERSION,    /* plugin version */
    YAMMER_PLUGIN_SUMMARY,    /* plugin summary */
    YAMMER_PLUGIN_DESCRIPTION,/* plugin description */
    YAMMER_PLUGIN_AUTHOR,     /* author */
    YAMMER_PLUGIN_HOMEPAGE,   /* website */
    plugin_load,              /* plugin load callback */
    plugin_unload,            /* plugin unload callback */
    NULL,                     /* plugin destroy callback */
    NULL,                     /* ui info */
    &prpl_info,               /* protocol info */
    NULL,                     /* preferences info */
    NULL,                     /* ui actions */
    NULL,                     /* reserved */
    NULL,                     /* reserved */
    NULL,                     /* reserved */
    NULL                      /* reserved */
};

static void
init_plugin(PurplePlugin *plugin)
{
  purple_debug_info(YAMMER_PLUGIN_ID, "plugin init\n");
}

PURPLE_INIT_PLUGIN(yammer, init_plugin, info);
