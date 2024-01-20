cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_STANDARD 11)
project(LogosApp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")


add_subdirectory(Sandbox)
add_subdirectory(Engine)


