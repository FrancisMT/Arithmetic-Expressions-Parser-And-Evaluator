project(Arithmetic-Expressions-Parser-Evaluator)

include_directories(./)
add_subdirectory(Parser)
add_subdirectory(Evaluator)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE parser
    PRIVATE evaluator
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
