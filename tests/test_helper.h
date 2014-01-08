#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <glib.h>
#include <json-glib/json-glib.h>

void      init_libpurple (void);
gboolean  is_env_loaded(void);
gchar*    env_test_token(void);
JsonNode* json_from_fixture(char* fixture_name);

#endif /* TEST_HELPER_H */
