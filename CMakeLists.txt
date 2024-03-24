cmake_minimum_required(VERSION 3.27.7)

project(library_manager)

find_package(SQLite3 REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_subdirectory(libs/log.c)
add_subdirectory(src)