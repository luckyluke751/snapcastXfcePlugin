cmake_minimum_required(VERSION 3.9)
project(snapcastXfcePlugin)

set(CMAKE_C_STANDARD 99)

include_directories(/usr/include/xfce4/)
INCLUDE_DIRECTORIES(/usr/include/xfce4/libxfce4panel-1.0)
include_directories(/usr/include/xfce4/libxfce4ui-2/)
include_directories(/usr/include/atk-1.0)
include_directories(/usr/include/gdk-pixbuf-2.0)
include_directories(/usr/include/cairo)
include_directories(/usr/include/pango-1.0/)
include_directories (/usr/include/gtk-3.0)
include_directories (/usr/include/glib-2.0)

include_directories(/usr/lib/glib-2.0/include)

link_directories(/usr/lib/)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

set(SOURCE_FILES library.c library.h)
add_library(snapcastXfcePlugin SHARED ${SOURCE_FILES})
target_link_libraries(snapcastXfcePlugin libxfce4ui-2.so libxfce4panel-1.0.so gtkmm-3.0 glib-2.0 ${GTK3_LIBRARIES})

