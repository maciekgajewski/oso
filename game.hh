#pragma once

#include "sdl.h"

namespace Oso {

class Game {
public:
    Game();

    // Run the main loop
    void run();

private:
    sdl::win_ptr _window;
};

} // namespace Oso
