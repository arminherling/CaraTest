find_package(Qt6 REQUIRED COMPONENTS Core)

set(CMAKE_AUTOMOC ON)

if(NOT Qt6_FOUND)
    message(FATAL_ERROR "Qt6 not found!")
endif()
