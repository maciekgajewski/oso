#include "game.hh"

namespace Oso {

Game::Game()
{
    _window = sdl::create_window("OWO", 0, 0, 800, 600, 0);
    _renderer = sdl::create_renderer(_window, -1, SDL_RENDERER_ACCELERATED);
}

void Game::run()
{
    ::SDL_Delay(1000);
}

} // namespace Oso
