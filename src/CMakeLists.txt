add_executable(${PROJECT_NAME} main.c book.c book_manager.c listbook.c)

target_link_directories(${PROJECT_NAME} 
    PUBLIC .
)
target_include_directories(${PROJECT_NAME} 
    PUBLIC .
    PUBLIC ${SQLite3_INCLUDE_DIRS}
    PUBLIC includes/
)
target_link_libraries(${PROJECT_NAME}
    ${SQLite3_LIBRARIES}
    logc
)