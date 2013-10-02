cmake_minimum_required (VERSION 2.8.2)
project (sshboardtrayicon C)


find_package(PkgConfig REQUIRED)
pkg_search_module(LIBNOTIFY REQUIRED libnotify)

pkg_check_modules(GTK3 gtk+-3.0)
  if (NOT ${GTK3_FOUND})
  message(FATAL_ERROR "GTK3 not found ")
endif ()

add_executable(sshdrop-trayicon sshdrop-trayicon.c)
include_directories(${GTK3_INCLUDE_DIRS} ${LIBNOTIFY_INCLUDE_DIR})
target_link_libraries(sshdrop-trayicon ${GTK3_LIBRARIES} ${LIBNOTIFY_LIBRARIES})
install(TARGETS sshdrop-trayicon DESTINATION bin)
