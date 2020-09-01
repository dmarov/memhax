add_executable(sigmaker main.cpp ${modules_src})

configure_file(main.h.in main.h)

target_include_directories(sigmaker PUBLIC "../.." PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

set_property(TARGET sigmaker PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(sigmaker PRIVATE yaml-cpp)
target_link_libraries(sigmaker PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(sigmaker PUBLIC Boost::program_options)

install(TARGETS sigmaker DESTINATION bin)
