project(CaraTest)

file(GLOB SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/source/**/*.cpp")
file(GLOB HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/source/**/*.h")

qt_add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/source")
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)

if(WIN32)
    set(THREADS_PREFER_PTHREAD_FLAG OFF)
    find_package(Threads REQUIRED)
    target_link_libraries(CaraTest PRIVATE Threads::Threads)
else()
    target_link_libraries(CaraTest PRIVATE -pthread)
endif()

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
             BASE_NAME ${PROJECT_NAME}
             EXPORT_MACRO_NAME CARATEST_API
             EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/source/${PROJECT_NAME}/API.h
)
