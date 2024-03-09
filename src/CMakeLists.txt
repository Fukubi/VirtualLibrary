add_executable(${PROJECT_NAME} main.c book.c book_manager.c listbook.c main_window.c)

target_link_directories(${PROJECT_NAME} 
    PUBLIC .
    PUBLIC ${GTK3_LIBRARY_DIRS}
)
target_include_directories(${PROJECT_NAME} 
    PUBLIC .
    PUBLIC ${SQLite3_INCLUDE_DIRS}
    PUBLIC ${GTK3_INCLUDE_DIRS}
    PUBLIC includes/
)
target_link_libraries(${PROJECT_NAME}
    ${SQLite3_LIBRARIES}
    ${GTK3_LIBRARIES}
    logc
)

add_definitions(${GTK3_CFLAGS_OTHER})