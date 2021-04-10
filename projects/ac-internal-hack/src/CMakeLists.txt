cmake_minimum_required(VERSION 3.18.0)
project(ac-hack VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB modules_src
    "../../../src/modules/*.cpp"
    "../../../src/exceptions/*.cpp"
)

add_library(ac-internal-hack SHARED main.cpp)

configure_file(main.h.in main.h)

target_include_directories(ac-internal-hack PUBLIC ".." PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

set_property(TARGET ac-internal-hack PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

install(TARGETS ac-internal-hack DESTINATION lib)
