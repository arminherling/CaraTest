project(AalTestSnapshotTool)

file(GLOB SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/source/*.cpp")
file(GLOB HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/source/*.h")

set(CMAKE_VS_JUST_MY_CODE_DEBUGGING ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

qt_add_resources(${PROJECT_NAME} styleresources
    FILES "styles/darkmode.qss" "styles/acceptbutton.qss" "styles/rejectbutton.qss"
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE AalTest Qt6::Widgets Dwmapi)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE AalTest Qt6::Widgets)
endif()


set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
)

if (WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif ()
