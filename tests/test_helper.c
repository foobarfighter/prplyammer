#include "test_helper.h"
#include <purple.h>
#include <stdio.h>
#include <cometd.h>
#include <stdlib.h>

#define CUSTOM_PLUGIN_PATH "/Users/bremeika/workspace/prplyammer/build/src"
#define UI_ID              "test_runner"

/**
 * The following eventloop functions are used in both pidgin and purple-text. If your
 * application uses glib mainloop, you can safely use this verbatim.
 */
#define PURPLE_GLIB_READ_COND  (G_IO_IN | G_IO_HUP | G_IO_ERR)
#define PURPLE_GLIB_WRITE_COND (G_IO_OUT | G_IO_HUP | G_IO_ERR | G_IO_NVAL)

typedef struct _PurpleGLibIOClosure {
  PurpleInputFunction function;
  guint result;
  gpointer data;
} PurpleGLibIOClosure;


gboolean is_env_loaded(void)
{
  gchar* client_id = getenv("CLIENT_ID");
  gchar* client_secret = getenv("CLIENT_SECRET");
  gchar* test_token = getenv("TEST_TOKEN");

  return client_id != NULL && client_secret != NULL && test_token != NULL;
}

gchar* env_test_token(void)
{
  return getenv("TEST_TOKEN");
}

gchar* read_file(char* path)
{
  char* buffer = 0;
  long length;
  FILE* f = fopen (path, "rb");

  if (f)
  {
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    buffer = malloc (length+1);
    if (buffer)
    {
      fread (buffer, 1, length, f);
      buffer[length] = '\0';
    }
    fclose (f);
  }

  return buffer;
}

static gchar* fixture_path = NULL;

JsonNode*
json_from_fixture(char* fixture_name)
{
  if (fixture_path == NULL)
    fixture_path = getenv("FIXTURE_PATH");

  g_assert(fixture_path != NULL);

  JsonNode* n;
  char* contents;
  char path[1024];

  sprintf(path, "%s/%s.json", fixture_path, fixture_name);
  contents = read_file(path);

  if (contents == NULL)
    printf("Can't read file at path: %s\n", path);

  n = cometd_json_str2node(contents);
  g_assert(n != NULL);

  free(contents);
  return n;
}

static void purple_glib_io_destroy(gpointer data)
{
  g_free(data);
}

static gboolean purple_glib_io_invoke(GIOChannel *source, GIOCondition condition, gpointer data)
{
  PurpleGLibIOClosure *closure = data;
  PurpleInputCondition purple_cond = 0;

  if (condition & PURPLE_GLIB_READ_COND)
    purple_cond |= PURPLE_INPUT_READ;
  if (condition & PURPLE_GLIB_WRITE_COND)
    purple_cond |= PURPLE_INPUT_WRITE;

  closure->function(closure->data, g_io_channel_unix_get_fd(source),
        purple_cond);

  return TRUE;
}

static guint glib_input_add(gint fd, PurpleInputCondition condition, PurpleInputFunction function,
                 gpointer data)
{
  PurpleGLibIOClosure *closure = g_new0(PurpleGLibIOClosure, 1);
  GIOChannel *channel;
  GIOCondition cond = 0;

  closure->function = function;
  closure->data = data;

  if (condition & PURPLE_INPUT_READ)
    cond |= PURPLE_GLIB_READ_COND;
  if (condition & PURPLE_INPUT_WRITE)
    cond |= PURPLE_GLIB_WRITE_COND;

#if defined _WIN32 && !defined WINPIDGIN_USE_GLIB_IO_CHANNEL
  channel = wpurple_g_io_channel_win32_new_socket(fd);
#else
  channel = g_io_channel_unix_new(fd);
#endif
  closure->result = g_io_add_watch_full(channel, G_PRIORITY_DEFAULT, cond,
                purple_glib_io_invoke, closure, purple_glib_io_destroy);

  g_io_channel_unref(channel);
  return closure->result;
}

static PurpleEventLoopUiOps glib_eventloops =
{
  g_timeout_add,
  g_source_remove,
  glib_input_add,
  g_source_remove,
  NULL,
#if GLIB_CHECK_VERSION(2,14,0)
  g_timeout_add_seconds,
#else
  NULL,
#endif

  /* padding */
  NULL,
  NULL,
  NULL
};
/*** End of the eventloop functions. ***/


static PurpleConversationUiOps null_conv_uiops =
{
  NULL,                      /* create_conversation  */
  NULL,                      /* destroy_conversation */
  NULL,                      /* write_chat           */
  NULL,                      /* write_im             */
  NULL,                      /* write_conv           */
  NULL,                      /* chat_add_users       */
  NULL,                      /* chat_rename_user     */
  NULL,                      /* chat_remove_users    */
  NULL,                      /* chat_update_user     */
  NULL,                      /* present              */
  NULL,                      /* has_focus            */
  NULL,                      /* custom_smiley_add    */
  NULL,                      /* custom_smiley_write  */
  NULL,                      /* custom_smiley_close  */
  NULL,                      /* send_confirm         */
  NULL,
  NULL,
  NULL,
  NULL
};

static void
null_ui_init(void)
{
  /**
   * This should initialize the UI components for all the modules. Here we
   * just initialize the UI for conversations.
   */
  purple_conversations_set_ui_ops(&null_conv_uiops);
}

static PurpleCoreUiOps null_core_uiops =
{
  NULL,
  NULL,
  null_ui_init,
  NULL,

  /* padding */
  NULL,
  NULL,
  NULL,
  NULL
};

void
init_libpurple (void)
{
  /* Set a custom user directory (optional) */
  // purple_util_set_user_dir(CUSTOM_USER_DIRECTORY);

  /* We do not want any debugging for now to keep the noise to a minimum. */
  purple_debug_set_enabled(TRUE);

  /* Set the core-uiops, which is used to
   *  - initialize the ui specific preferences.
   *  - initialize the debug ui.
   *  - initialize the ui components for all the modules.
   *  - uninitialize the ui components for all the modules when the core terminates.
   */
  purple_core_set_ui_ops(&null_core_uiops);

  /* Set the uiops for the eventloop. If your client is glib-based, you can safely
   * copy this verbatim. */
  purple_eventloop_set_ui_ops(&glib_eventloops);

  /* Set path to search for plugins. The core (libpurple) takes care of loading the
   * core-plugins, which includes the protocol-plugins. So it is not essential to add
   * any path here, but it might be desired, especially for ui-specific plugins. */
  printf("loading %s\n", CUSTOM_PLUGIN_PATH);
  purple_plugins_add_search_path(CUSTOM_PLUGIN_PATH);

  /* Now that all the essential stuff has been set, let's try to init the core. It's
   * necessary to provide a non-NULL name for the current ui to the core. This name
   * is used by stuff that depends on this ui, for example the ui-specific plugins. */
  if (!purple_core_init(UI_ID)) {
    /* Initializing the core failed. Terminate. */
    fprintf(stderr,
        "libpurple initialization failed. Dumping core.\n"
        "Please report this!\n");
    abort();
  }

  /* Create and load the buddylist. */
  purple_set_blist(purple_blist_new());
  purple_blist_load();

  /* Load the preferences. */
  // purple_prefs_load();

  /* Load the desired plugins. The client should save the list of loaded plugins in
   * the preferences using purple_plugins_save_loaded(PLUGIN_SAVE_PREF) */
  // purple_plugins_load_saved(PLUGIN_SAVE_PREF);

  /* Load the pounces. */
  purple_pounces_load();

}
