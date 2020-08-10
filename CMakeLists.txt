cmake_minimum_required(VERSION 3.18.0)
project(sigmaker VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(main.h.in main.h)

file(GLOB modules_src
    "src/modules/*.cpp"
)

add_executable(${PROJECT_NAME} src/main.cpp ${modules_src})

target_include_directories(${PROJECT_NAME} PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

                       set_property(TARGET ${PROJECT_NAME} PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

find_package(yaml-cpp CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::program_options)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
