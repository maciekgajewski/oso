#include "game.hh"

namespace Oso {

using namespace std::literals;

Game::Game()
    : _camera(400, 300)
{
    _window = sdl::create_window("OWO", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    _renderer = sdl::create_renderer(_window, -1, SDL_RENDERER_ACCELERATED);

    _buffer = sdl::create_texture(_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 400, 300);

    World::id_t earthId = _world.addPlanet("Earth");
    Planet& earth = _world.getPlanet(earthId);
    earth.generate(10000, 1000);
    earth.rotation = 10;

    World::id_t moonId = _world.addPlanet("Moon");
    Planet& moon = _world.getPlanet(moonId);
    moon.generate(2000, 200);
    moon.rotation = 30;
    position_t& moonPos = _world.getPosition(moonId);
    moonPos.x = 50000;

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
        tick_t oldTime = _simTime;
        _simTime += 100ms;

        render();
        updateTime(oldTime, _simTime);
    }
}

void Game::render()
{
    int r = ::SDL_SetRenderTarget(_renderer, _buffer);
    if (r != 0)
        sdl::throw_error("setting render target");
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(_renderer);

    _world.visitPlanets([&](const location_t& position, const Planet& planet) {
        planet.render(_camera, _renderer, position);
    });

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

void Game::updateTime(tick_t oldTime, tick_t currentTime)
{
    tick_t dt = currentTime - oldTime;

    _world.visitPlanets([&](location_t& position, Planet& planet) {
        position.a += planet.rotation * dt.count() * 0.001;
    });
}

} // namespace Oso
