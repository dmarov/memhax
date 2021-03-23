add_library(ac-internal-hack SHARED main.cpp ${modules_src})

configure_file(main.h.in main.h)

target_include_directories(ac-internal-hack PUBLIC "../.." PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

set_property(TARGET ac-internal-hack PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(ac-internal-hack PRIVATE yaml-cpp)
target_link_libraries(ac-internal-hack PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(ac-internal-hack PUBLIC Boost::program_options)

install(TARGETS ac-internal-hack DESTINATION bin)
