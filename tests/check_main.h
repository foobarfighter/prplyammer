#ifndef CHECK_MAIN_H
#define CHECK_MAIN_H

#include <check.h>
#include "test_helper.h"
#include "libyammer.h"

Suite* make_libyammer_suite (void);
Suite* make_yammer_request_suite (void);

#endif /* CHECK_MAIN_H */
