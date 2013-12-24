#include "libyammer.h"

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

static PurplePluginProtocolInfo prpl_info = {
  /* options */
  OPT_PROTO_MAIL_CHECK,

  NULL,                   /* user_splits */
  NULL,                   /* protocol_options */
  /* NO_BUDDY_ICONS */    /* icon_spec */
  {"jpg", 0, 0, 50, 50, -1, PURPLE_ICON_SCALE_SEND}, /* icon_spec */
  NULL,                   /* list_icon */
  NULL,                   /* list_emblems */
  NULL,                   /* status_text */
  NULL,                   /* tooltip_text */
  NULL,                   /* status_types */
  NULL,                   /* blist_node_menu */
  NULL,                   /* chat_info */
  NULL,                   /* chat_info_defaults */
  NULL,                   /* login */
  NULL,                   /* close */
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
}

PURPLE_INIT_PLUGIN(hello_world, init_plugin, info);