cmake_minimum_required(VERSION 3.28)

project(library_manager)

find_package(SQLite3 REQUIRED)

add_subdirectory(libs/log.c)
add_subdirectory(src)