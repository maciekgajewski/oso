#pragma once

#include "sdl.h"

namespace Oso {

class Game {
public:
    Game();

    // Run the main loop
    void run();

private:
    sdl::window _window;
    sdl::renderer _renderer;
};

} // namespace Oso
