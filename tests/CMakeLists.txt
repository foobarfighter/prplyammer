find_package(CHECK REQUIRED)
find_package(LIBPURPLE REQUIRED)
find_package(GLIB2 REQUIRED)
find_package(JsonGlib REQUIRED)

include_directories (../include
                     ${LIBPURPLE_INCLUDE_DIRS}
                     ${GLIB2_INCLUDE_DIR}
                     ${JsonGlib_INCLUDE_DIRS}
                     ${CHECK_INCLUDE_DIR})

add_executable(check_main check_main.c
                          test_helper.c
                          check_prplyammer.c
                          check_yammer_request.c
                          check_yammer_response.c
                          check_yammer_api.c)

target_link_libraries(check_main prplyammer ${CHECK_LIBRARIES})

add_test(check_main ${CMAKE_CURRENT_BINARY_DIR}/check_main)
