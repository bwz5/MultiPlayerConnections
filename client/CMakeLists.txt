cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(client)

add_subdirectory( lib/enet)
add_subdirectory(lib/raylib)

set(CLIENT_SOURCES src/main.cpp
                    src/Game.cpp)

add_executable(client ${CLIENT_SOURCES})

target_include_directories(client PUBLIC lib/enet/include lib/raylib/src)
target_link_libraries(client enet raylib)