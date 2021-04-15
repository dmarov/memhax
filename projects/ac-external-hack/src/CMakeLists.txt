cmake_minimum_required(VERSION 3.18.0)
project(ac-hack VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB modules_src
    "../../../src/modules/*.cpp"
    "../../../src/exceptions/*.cpp"
    "../../../src/loops/*.cpp"
)

find_package(Boost REQUIRED COMPONENTS program_options)

add_executable(ac-hack main.cpp ${modules_src})

configure_file(main.h.in main.h)

target_include_directories(ac-hack PUBLIC "../../../src" PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

set_property(TARGET ac-hack PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(ac-hack PUBLIC Boost::program_options)

install(TARGETS ac-hack DESTINATION "bin")
