cmake_minimum_required (VERSION 2.8.2)
project (sshboardtrayicon C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBNOTIFY REQUIRED libnotify)
pkg_check_modules(LIBQRENCODE REQUIRED libqrencode)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(LIBCANBERRA REQUIRED libcanberra-gtk3)

add_executable(sshdrop-trayicon sshdrop-trayicon.c)
include_directories(${GTK3_INCLUDE_DIRS} ${LIBNOTIFY_INCLUDE_DIR} ${LIBCANBERRA_INCLUDE_DIRS})
target_link_libraries(sshdrop-trayicon ${GTK3_LIBRARIES} ${LIBNOTIFY_LIBRARIES} ${LIBQRENCODE_LIBRARIES} ${LIBCANBERRA_LIBRARIES})
install(TARGETS sshdrop-trayicon DESTINATION bin)

add_executable(textqr qr/qr.cc)
target_link_libraries(textqr qrencode)
set_target_properties(textqr PROPERTIES COMPILE_FLAGS -std=c++11)

