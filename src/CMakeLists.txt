find_package(LIBPURPLE REQUIRED)
find_package(GLIB2 REQUIRED)

include_directories (../include
                     ${LIBPURPLE_INCLUDE_DIR}
                     ${GLIB2_INCLUDE_DIR})

add_library(yammer libyammer.c)

target_link_libraries(yammer ${LIBPURPLE_LIBRARIES})
