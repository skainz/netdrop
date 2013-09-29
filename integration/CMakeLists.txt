cmake_minimum_required (VERSION 2.8.2)

project (sshboardtrayicon)

set (sshboardtrayicon_VERSION_MAJOR 0)
set (sshboardtrayicon_VERSION_MAJOR 1)

add_executable(sshboard-trayicon sshboard-trayicon.c)

include(UsePkgConfig)

PKGCONFIG(gtk+-2.0 GTK2_INCLUDE_DIR GTK2_LINK_DIR GTK2_LINK_FLAGS
                   GTK2_CFLAGS)

find_package(GTK2 2.6 REQUIRED gtk)

if(GTK2_FOUND)
      include_directories(${GTK2_INCLUDE_DIRS})
      target_link_libraries(sshboard-trayicon ${GTK2_LIBRARIES})
   endif()

SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${LCB_CFLAGS})

include_directories(${LCB_INCLUDE_DIR})
target_link_libraries(sshboard-trayicon notify)
target_link_libraries(sshboard-trayicon ${LCB_LINK_FLAGS})
