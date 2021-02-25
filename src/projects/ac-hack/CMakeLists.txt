add_executable(ac-hack main.cpp ${modules_src})

configure_file(main.h.in main.h)

target_include_directories(ac-hack PUBLIC "../.." PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

set_property(TARGET ac-hack PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(ac-hack PRIVATE yaml-cpp)
target_link_libraries(ac-hack PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(ac-hack PUBLIC Boost::program_options)

install(TARGETS ac-hack DESTINATION bin)
