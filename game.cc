#include "game.hh"

#include <iostream>
#include <memory>

namespace Oso {

using namespace std::literals;

Game::Game() : _camera(400, 300), _gui(*this) {
  _window = sdl::create_window("OWO", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
  _renderer = sdl::create_renderer(_window, -1, SDL_RENDERER_ACCELERATED);

  _buffer = sdl::create_texture(_renderer, SDL_PIXELFORMAT_RGBA32,
                                SDL_TEXTUREACCESS_TARGET, 400, 300);

  entity_id earthId = _world.addPlanet("Earth", 5.9724E16);
  Planet &earth = _world.getPlanet(earthId);
  earth.generate(10000, 1000);
  earth.rotation = 0.01;

  Orbit moonOrbit = Orbit::circular(50000, 0);
  entity_id moonId =
      _world.addOrbitingPlanet("Moon", 5.9724E14, earthId, moonOrbit);
  Planet &moon = _world.getPlanet(moonId);
  moon.generate(2000, 200);
  moon.rotation = 0.03;

  // add some asteroids
  entity_id asteroidId = _world.addAsteroid(
      "a1", 1E4, earthId, velocity_v{5, 12}, location_t{{30000, 0}, 0});
  Planet &asteroid = _world.getPlanet(asteroidId);
  asteroid.rotation = 0.1;
  asteroid.generate(200, 50);

  _camera.setZoomLevel(-9);
}

void Game::run() {
  ::SDL_Event e;
  while (true) {
    while (::SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        return;
      }

      if (!_gui.onEvent(e))
        onCameraEvent(e);
    }

    ::SDL_Delay(60);

    _realTime += 60ms;
    if (!_paused) {
      tick_t oldTime = _simTime;
      _simTime += _timeAccel * 60ms;
      _gui.update();
      render();
      updateTime(oldTime, _simTime);
    } else {
      _gui.update();
      render();
    }
  }
}

void renderOrbit(const Camera &cam, SDL_Renderer *renderer, position_t center,
                 Orbit orbit) {

  const int SEGMENTS = 36;
  std::vector<SDL_Point> points;
  points.reserve(SEGMENTS + 1);

  for (int i = 0; i < SEGMENTS; ++i) {
    angle_t tau = i * M_PI / 18; // iterate over eccentric anomaly
    double ct = std::cos(tau);
    double st = std::sin(tau);

    distnce_t r = orbit.a * (1 - orbit.e * ct);
    angle_t o = std::atan2(std::sqrt(1 - orbit.e * orbit.e) * st, ct - orbit.e);

    position_t phys = center + r * position_t{std::cos(o), std::sin(o)};
    points.push_back(cam.physToScreen(phys));
  }
  points.push_back(points.front());

  SDL_SetRenderDrawColor(renderer, 0, 0, 64, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLines(renderer, points.data(), SEGMENTS + 1);
}

void Game::render() {

  // render on buffer

  int r = ::SDL_SetRenderTarget(_renderer, _buffer);
  if (r != 0)
    sdl::throw_error("setting render target");
  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(_renderer);

  // orbits
  _world.visitForcedOrbits([&](location_t &position, Comp::ForcedOrbit &orbit) {
    position_t pos = _world.getPosition(orbit.parent).pos;
    renderOrbit(_camera, _renderer, pos, orbit.orbit);
  });

  // planets
  _world.visitPlanets([&](const location_t &position, const Planet &planet) {
    planet.render(_camera, _renderer, position);
  });

  // render gui
  _gui.render(_renderer);

  SDL_RenderPresent(_renderer);

  // render rescaled buffer on screen
  r = ::SDL_SetRenderTarget(_renderer, nullptr);
  if (r != 0)
    sdl::throw_error("setting render target");
  SDL_SetRenderDrawColor(_renderer, 200, 0, 200, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(_renderer);
  SDL_Rect src{0, 0, 400, 300};
  SDL_Rect dst{0, 0, 800, 600};
  SDL_RenderCopy(_renderer, _buffer, &src, &dst);
  SDL_RenderPresent(_renderer);
}

void Game::updateTime(tick_t oldTime, tick_t currentTime) {
  tick_t dt = currentTime - oldTime;

  _world.visitPlanets([&](location_t &position, Planet &planet) {
    position.a += planet.rotation * dt.count() * 0.001;
  });

  _world.visitForcedOrbits(
      [&](location_t &position, Comp::ForcedOrbit &planet) {
        mass_t M = _world.getGravitySource(planet.parent).mass;
        position.pos = planet.orbit.findPosition(currentTime, 0, M);
      });

  // physics simlualtion

  // calculate gravity
  _world.visitPhysSimulated(
      [&](location_t &position, Comp::PhysSimulated &simulated) {
        simulated.accel = {0, 0};

        _world.visitGravitySources(
            [&](location_t &gsLocation, const Comp::GravitySource &gs) {
              auto rv = position.pos - gsLocation.pos;
              double r2 = (rv.x * rv.x + rv.y * rv.y);
              distnce_t r = std::sqrt(r2);
              accel_t a = G * gs.mass / r2;
              accel_v av = -a / r * rv;
              simulated.accel += av;
            });

        simulated.velocity += 0.001 * dt.count() * simulated.accel;
        position.pos += 0.001 * dt.count() * simulated.velocity;
      });
}

bool Game::onCameraEvent(const SDL_Event &e) {

  if (e.type == SDL_MOUSEWHEEL) {
    if (e.wheel.y < 0 && _camera.getZoomLevel() > -20)
      _camera.setZoomLevel(_camera.getZoomLevel() - 1);
    else if (e.wheel.y > 0 && _camera.getZoomLevel() < 0)
      _camera.setZoomLevel(_camera.getZoomLevel() + 1);

    return true;
  }

  return false;
}

} // namespace Oso
