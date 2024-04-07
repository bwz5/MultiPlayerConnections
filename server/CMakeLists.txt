cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(server)

add_subdirectory( lib/enet )

set(SERVER_SOURCES src/main.cpp
                    src/Game.cpp)

add_executable(server ${SERVER_SOURCES})

target_include_directories(server PUBLIC lib/enet/include )
target_link_libraries(server enet )