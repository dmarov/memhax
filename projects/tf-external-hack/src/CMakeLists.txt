cmake_minimum_required(VERSION 3.18.0)
project(dll-injector VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB modules_src
    "../../../src/modules/*.cpp"
    "../../../src/exceptions/*.cpp"
)

add_executable(tf-hack main.cpp ${modules_src})

configure_file(main.h.in main.h)

target_include_directories(tf-hack PUBLIC "../../../src" PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

set_property(TARGET tf-hack PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

install(TARGETS tf-hack DESTINATION bin)
