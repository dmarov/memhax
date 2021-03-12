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

# add_subdirectory(src/projects/tf-hack tf-hack)
# add_subdirectory(src/projects/tf-internal-hack tf-internal-hack)
add_subdirectory(src/projects/tf-internal-hack tf-internal-hack)
add_subdirectory(src/projects/dll-injector dll-injector)
# add_subdirectory(src/projects/sigmaker sigmaker)
# add_subdirectory(src/projects/sigmaker-monitor sigmaker-monitor)
# add_subdirectory(src/projects/ac-hack ac-hack)
