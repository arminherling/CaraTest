cmake_minimum_required(VERSION 3.20)

project(AalTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

add_subdirectory(AalTest)

add_subdirectory(Tests/AalTestTests)
