cmake_minimum_required(VERSION 3.4.1)

project(timer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CXX_FLAGS
 -O2
 -std=c++11
)

if(CMAKE_BUILD_BITS EQUAL 32)
    list(APPEND CXX_FLAGS "-m32")
else()
    list(APPEND CXX_FLAGS "-m64")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

aux_source_directory(${PROJECT_SOURCE_DIR} src)

add_library(${PROJECT_NAME} STATIC ${src})

add_subdirectory(test)
