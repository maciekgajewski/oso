#include "game.hh"

namespace Oso {

using namespace std::literals;

Game::Game()
    : _camera(800, 600)
{
    _window = sdl::create_window("OWO", 0, 0, 800, 600, 0);
    _renderer = sdl::create_renderer(_window, -1, SDL_RENDERER_ACCELERATED);

    _planet.generate(10000, 1000);
    _camera.setZoomLevel(-6);
}

void Game::run()
{
    ::SDL_Event e;
    while (true) {
        while (::SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return;
            }
        }

        ::SDL_Delay(100);
        _realTime += 100ms;
        _simTime += 100ms;

        render();
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(_renderer);

    _planet.render(_camera, _renderer);
    SDL_RenderPresent(_renderer);
}

} // namespace Oso
