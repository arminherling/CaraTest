cmake_minimum_required(VERSION 3.20)

project(CaraTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_VS_JUST_MY_CODE_DEBUGGING ON)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

add_subdirectory(CaraTest)

option(CARATEST_BUILD_TESTS "Enable building tests." OFF)
if(CARATEST_BUILD_TESTS)
    add_subdirectory(Tests/AllInOne)
    add_subdirectory(Tests/CaraTestTests)
    add_subdirectory(Tests/DiffTests)
endif()

option(CARATEST_BUILD_TOOLS "Enable building tools." OFF)
if(CARATEST_BUILD_TOOLS)
    add_subdirectory(Tools/CaraTestSnapshotTool)
endif()
