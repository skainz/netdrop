cmake_minimum_required (VERSION 2.8.2)
project (sshboardtrayicon C)

find_package(GTK2 2.6 REQUIRED gtk)
find_package(PkgConfig REQUIRED)
pkg_search_module(LIBNOTIFY REQUIRED libnotify)

add_executable(sshdrop-trayicon sshdrop-trayicon.c)
include_directories(${GTK2_INCLUDE_DIRS} ${LIBNOTIFY_INCLUDE_DIR})
target_link_libraries(sshdrop-trayicon ${GTK2_LIBRARIES} ${LIBNOTIFY_LIBRARIES})
