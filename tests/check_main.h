#ifndef CHECK_MAIN_H
#define CHECK_MAIN_H

#include <check.h>
#include "test_helper.h"
#include "prplyammer.h"

Suite* make_prplyammer_suite (void);
Suite* make_yammer_request_suite (void);
Suite* make_yammer_response_suite (void);
Suite* make_yammer_api_suite (void);

#endif /* CHECK_MAIN_H */
