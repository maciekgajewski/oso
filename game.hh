#pragma once

#include "camera.hh"
#include "planet.hh"
#include "sdl.h"
#include "types.hh"

namespace Oso {

class Game {
public:
    Game();

    // Run the main loop
    void run();

private:
    void render();

    sdl::window _window;
    sdl::renderer _renderer;

    tick_t _realTime = tick_t(0);
    tick_t _simTime = tick_t(0);

    Camera _camera;
    Planet _planet;
};

} // namespace Oso
