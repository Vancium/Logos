


set(CMAKE_C_STANDARD 11)

add_executable(LogosApp src/main.c)

set(LINK_OPTIONS "-L../bin;-lengine;-Wl;-rpath")
set_source_files_properties(src/main.c PROPERTIES COMPILE_OPTIONS "-g;-fPIC")
target_link_libraries(LogosApp LogosEngine)

add_definitions(-D_DEBUG -DLEXPORT)
