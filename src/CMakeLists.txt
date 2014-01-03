find_package(LIBPURPLE REQUIRED)
find_package(GLIB2 REQUIRED)

include_directories (../include
                     ${LIBPURPLE_INCLUDE_DIR}
                     ${GLIB2_INCLUDE_DIR})

add_library(prplyammer SHARED prplyammer.c
                              yammer_request.c
                              yammer_response.c
                              yammer_account.c)

if (APPLE)
    set_property(TARGET prplyammer PROPERTY OUTPUT_NAME "prplyammer.so")
    set_property(TARGET prplyammer PROPERTY SUFFIX "")
endif()

target_link_libraries(prplyammer ${LIBPURPLE_LIBRARIES}
                                 ${GLIB2_LIBRARIES})

#install(TARGETS prplyammer DESTINATION ~/.purple/plugins)
#install(TARGETS prplyammer DESTINATION /opt/local/lib/purple-2)
install(TARGETS prplyammer DESTINATION /usr/local/lib/purple-2)
