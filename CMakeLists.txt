cmake_minimum_required(VERSION 3.20)

project(AalTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

add_subdirectory(AalTest)

option(AALTEST_BUILD_TESTS "Enable building tests." OFF)
if(AALTEST_BUILD_TESTS)
    add_subdirectory(Tests/AllInOne)
    add_subdirectory(Tests/AalTestTests)
    add_subdirectory(Tests/DiffTests)
endif()

option(AALTEST_BUILD_TOOLS "Enable building tools." OFF)
if(AALTEST_BUILD_TOOLS)
    add_subdirectory(Tools/AalTestSnapshotTool)
endif()
