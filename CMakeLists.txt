cmake_minimum_required(VERSION 3.30)

project(CaraTest LANGUAGES CXX VERSION 2025.11.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

option(CARATEST_BUILD_TESTS "Enable building tests." OFF)
option(CARATEST_BUILD_TOOLS "Enable building tools." OFF)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(Qt6)


add_subdirectory(CaraTest)

if(CARATEST_BUILD_TESTS)
    add_subdirectory(Tests/AllInOne)
    add_subdirectory(Tests/CaraTestTests)
    add_subdirectory(Tests/DiffTests)
endif()

if(CARATEST_BUILD_TOOLS)
    add_subdirectory(Tools/CaraTestSnapshotTool)

    # Installiere das Tool
    install(
        TARGETS CaraTestSnapshotTool
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_VS_JUST_MY_CODE_DEBUGGING ON)
endif()


configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/CMake/caratest.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/caratest.pc
  @ONLY
)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/caratest.pc
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/caratest-config-version.cmake"
    VERSION ${CARATEST_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/caratest-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CaraTest"
)

