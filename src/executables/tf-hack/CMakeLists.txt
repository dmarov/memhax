add_executable(tf-hack main.cpp ${modules_src})

configure_file(main.h.in main.h)

target_include_directories(tf-hack PUBLIC "../..")

set_property(TARGET tf-hack PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(tf-hack PRIVATE yaml-cpp)
target_link_libraries(tf-hack PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(tf-hack PUBLIC Boost::program_options)

install(TARGETS tf-hack DESTINATION bin)
