#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <glib.h>

void     init_libpurple (void);
gboolean is_env_loaded(void);
gchar*   env_test_token(void);

#endif /* TEST_HELPER_H */
