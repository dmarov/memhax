cmake_minimum_required(VERSION 3.18.0)
project(ac-internal-hack VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB modules_src
    "../../../src/modules/*.cpp"
    "../../../src/exceptions/*.cpp"
    "../../../src/loops/*.cpp"
)

if (MSVC)
    add_compile_options(/std:c++17)
endif()

add_library(ac-internal-hack SHARED main.cpp ${modules_src})

configure_file(main.h.in main.h)

target_include_directories(ac-internal-hack PUBLIC "../../../src" PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

set_property(TARGET ac-internal-hack PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

install(TARGETS ac-internal-hack DESTINATION lib)
