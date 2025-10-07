cmake_minimum_required(VERSION 3.10)
project(jeu-graphe-sfml)
set(CMAKE_CXX_STANDARD 11)
find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Window System)

include_directories(src)
file(GLOB SOURCES
    src/*.cpp
    src/ui/*.cpp
)

add_executable(jeu-graphe-sfml ${SOURCES})
target_link_libraries(jeu-graphe-sfml PRIVATE SFML::Graphics SFML::Window SFML::System)