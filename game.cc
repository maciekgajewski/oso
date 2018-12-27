#include "game.hh"

namespace Oso {

using namespace std::literals;

Game::Game()
    : _camera(400, 300)
{
    _window = sdl::create_window("OWO", 0, 0, 800, 600, 0);
    _renderer = sdl::create_renderer(_window, -1, SDL_RENDERER_ACCELERATED);

    _buffer = sdl::create_texture(_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 400, 300);

    _planet.generate(10000, 1000);
    _camera.setZoomLevel(-9);
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
    int r = ::SDL_SetRenderTarget(_renderer, _buffer);
    if (r != 0)
        sdl::throw_error("setting render target");
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(_renderer);

    _planet.render(_camera, _renderer);
    SDL_RenderPresent(_renderer);

    r = ::SDL_SetRenderTarget(_renderer, nullptr);
    if (r != 0)
        sdl::throw_error("setting render target");
    SDL_SetRenderDrawColor(_renderer, 200, 0, 200, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(_renderer);
    SDL_Rect src{ 0, 0, 400, 300 };
    SDL_Rect dst{ 0, 0, 800, 600 };
    SDL_RenderCopy(_renderer, _buffer, &src, &dst);
    SDL_RenderPresent(_renderer);
}

} // namespace Oso
