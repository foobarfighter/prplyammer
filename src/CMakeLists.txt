find_package(LIBPURPLE REQUIRED)
find_package(GLIB2 REQUIRED)

include_directories (../include
                     ${LIBPURPLE_INCLUDE_DIR}
                     ${GLIB2_INCLUDE_DIR})

add_library(yaydium SHARED libyammer.c
                           yammer_request.c)

if (APPLE)
    set_property(TARGET yaydium PROPERTY OUTPUT_NAME "yaydium.so")
    set_property(TARGET yaydium PROPERTY SUFFIX "")
endif()

target_link_libraries(yaydium ${LIBPURPLE_LIBRARIES}
                              ${GLIB2_LIBRARIES})

#install(TARGETS yaydium DESTINATION ~/.purple/plugins)
#install(TARGETS yaydium DESTINATION /opt/local/lib/purple-2)
install(TARGETS yaydium DESTINATION /usr/local/lib/purple-2)
