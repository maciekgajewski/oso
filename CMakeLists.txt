project(oso)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

add_executable(oso
    main.cc
    game.cc game.hh
    sdl.cc sdl.h
    planet.cc planet.hh
    types.hh
    camera.cc camera.hh
)

target_include_directories(oso SYSTEM PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(oso ${SDL2_LIBRARIES})

