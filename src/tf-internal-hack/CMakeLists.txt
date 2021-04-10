add_library(tf-internal-hack SHARED main.cpp ${modules_src})

configure_file(main.h.in main.h)

target_include_directories(tf-internal-hack PUBLIC ".." PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

set_property(TARGET tf-internal-hack PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(tf-internal-hack PRIVATE yaml-cpp)
target_link_libraries(tf-internal-hack PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(tf-internal-hack PUBLIC Boost::program_options)

install(TARGETS tf-internal-hack DESTINATION lib)
