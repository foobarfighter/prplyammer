find_package(LIBPURPLE REQUIRED)
find_package(GLIB2 REQUIRED)
find_package(JsonGlib REQUIRED)
find_package(COMETD REQUIRED)

include_directories (../include
                     ${LIBPURPLE_INCLUDE_DIR}
                     ${GLIB2_INCLUDE_DIR}
                     ${JsonGlib_INCLUDE_DIRS}
                     ${COMETD_INCLUDE_DIRS})

add_library(prplyammer SHARED prplyammer.c
                              yammer_request.c
                              yammer_response.c
                              yammer_account.c
                              yammer_api.c
                              yammer_session.c)

if (APPLE)
    set_property(TARGET prplyammer PROPERTY OUTPUT_NAME "prplyammer.so")
    set_property(TARGET prplyammer PROPERTY SUFFIX "")
endif()

target_link_libraries(prplyammer ${LIBPURPLE_LIBRARIES}
                                 ${GLIB2_LIBRARIES}
                                 ${JsonGlib_LIBRARIES}
                                 ${COMETD_LIBRARIES})

#install(TARGETS prplyammer DESTINATION ~/.purple/plugins)
#install(TARGETS prplyammer DESTINATION /opt/local/lib/purple-2)
install(TARGETS prplyammer DESTINATION /usr/local/lib/purple-2)
