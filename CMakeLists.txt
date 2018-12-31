project(oso)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

set(SDL2_gfx_LIBRARIES SDL2_gfx)
set(SDL2_image_LIBRARIES SDL2_image)
set(SDL2_ttf_LIBRARIES SDL2_ttf)

add_executable(oso
    main.cc
    game.cc game.hh
    sdl.cc sdl.h
    planet.cc planet.hh
    types.hh
    camera.cc camera.hh
    world.cc world.hh
    orbit.cc orbit.hh
    components.hh
    gui_control.cc gui_control.hh
    resources.cc resources.hh
)

target_include_directories(oso SYSTEM PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(oso ${SDL2_LIBRARIES} ${SDL2_gfx_LIBRARIES} ${SDL2_image_LIBRARIES} ${SDL2_ttf_LIBRARIES})

