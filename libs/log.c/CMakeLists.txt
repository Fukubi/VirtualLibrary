add_library(logc src/log.c src/log.h)

target_include_directories(logc PUBLIC src)
target_link_directories(logc PRIVATE src)