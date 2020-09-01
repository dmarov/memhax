add_executable(sigmaker-monitor main.cpp ${modules_src})

configure_file(main.h.in main.h)

target_include_directories(sigmaker-monitor PUBLIC "../.." PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

set_property(TARGET sigmaker-monitor PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(sigmaker-monitor PRIVATE yaml-cpp)
target_link_libraries(sigmaker-monitor PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(sigmaker-monitor PUBLIC Boost::program_options)

install(TARGETS sigmaker-monitor DESTINATION bin)
