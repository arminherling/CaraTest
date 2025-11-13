project(CaraTest)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/source/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/source/*.h")
    
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>
    $<INSTALL_INTERFACE:include>
)

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC _USE_DETAILED_FUNCTION_NAME_IN_SOURCE_LOCATION=0)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8 /WX /diagnostics:caret)
endif()

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
             BASE_NAME ${PROJECT_NAME}
             EXPORT_MACRO_NAME CARATEST_API
             EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/source/${PROJECT_NAME}/API.h
)


install(TARGETS ${PROJECT_NAME}
    EXPORT CaraTestTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT CaraTestTargets
    FILE CaraTestTargets.cmake
    NAMESPACE CaraTest::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CaraTest
)
