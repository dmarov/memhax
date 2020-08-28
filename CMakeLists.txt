cmake_minimum_required(VERSION 3.18.0)
project(memhax VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(src/memhax.h.in memhax.h)

file(GLOB modules_src
    "src/modules/*.cpp"
)

add_custom_target(memhax ALL)

add_executable(tf-hack src/tf-hack.cpp ${modules_src})
add_executable(sigmaker src/sigmaker.cpp ${modules_src})
add_executable(sigmaker-monitor src/sigmaker-monitor.cpp ${modules_src})

target_include_directories(tf-hack PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(sigmaker PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(sigmaker-monitor PUBLIC "${PROJECT_BINARY_DIR}")

set_property(TARGET tf-hack PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET sigmaker PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET sigmaker-monitor PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

find_package(yaml-cpp CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

target_link_libraries(tf-hack PRIVATE yaml-cpp)
target_link_libraries(tf-hack PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(tf-hack PUBLIC Boost::program_options)

target_link_libraries(sigmaker PRIVATE yaml-cpp)
target_link_libraries(sigmaker PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(sigmaker PUBLIC Boost::program_options)

target_link_libraries(sigmaker-monitor PRIVATE yaml-cpp)
target_link_libraries(sigmaker-monitor PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(sigmaker-monitor PUBLIC Boost::program_options)

install(TARGETS tf-hack DESTINATION bin)
install(TARGETS sigmaker DESTINATION bin)
install(TARGETS sigmaker-monitor DESTINATION bin)
