find_package(CHECK REQUIRED)
find_package(LIBPURPLE REQUIRED)
find_package(GLIB2 REQUIRED)

include_directories (../include
                     ${LIBPURPLE_INCLUDE_DIR}
                     ${GLIB2_INCLUDE_DIR}
                     ${CHECK_INCLUDE_DIR})

add_executable(check_libyammer check_libyammer.c)

target_link_libraries(check_libyammer yaydium ${CHECK_LIBRARIES})

add_test(check_libyammer ${CMAKE_CURRENT_BINARY_DIR}/check_libyammer)
