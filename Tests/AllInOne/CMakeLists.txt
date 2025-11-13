project(AllInOne)

add_executable(
    ${PROJECT_NAME} 
    "${PROJECT_SOURCE_DIR}/../CaraTestTests/source/CaraTestTests.cpp"
    "${PROJECT_SOURCE_DIR}/../DiffTests/source/DiffTests.cpp"
    "source/main.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE CaraTest)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()
