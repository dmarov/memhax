cmake_minimum_required(VERSION 3.18.0)
project(winkmemdx64 VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../wdk")
find_package(WDK REQUIRED)
string(REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W1 /WX")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W1 /WX")

wdk_add_driver(winkmemdx64 KMDF 1.15 "main.cpp")

configure_file(
    "${CMAKE_SOURCE_DIR}/winkmemdx64.inf"
    "${CMAKE_CURRENT_BINARY_DIR}"
    COPYONLY
)
