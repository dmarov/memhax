cmake_minimum_required(VERSION 3.18.0)
project(memhax VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB modules_src
    "src/modules/*.cpp"
)

add_custom_target(memhax ALL)

find_package(yaml-cpp CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

add_subdirectory(src/executables/tf-hack tf-hack)
add_subdirectory(src/executables/sigmaker sigmaker)
add_subdirectory(src/executables/sigmaker-monitor sigmaker-monitor)
