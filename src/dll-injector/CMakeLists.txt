add_executable(dll-injector main.cpp ${modules_src})

configure_file(main.h.in main.h)

target_include_directories(dll-injector PUBLIC ".." PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

set_property(TARGET dll-injector PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(dll-injector PRIVATE yaml-cpp)
target_link_libraries(dll-injector PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(dll-injector PUBLIC Boost::program_options)

install(TARGETS dll-injector DESTINATION bin)
