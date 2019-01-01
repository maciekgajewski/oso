#pragma once

#include "camera.hh"
#include "game_gui.hh"
#include "sdl.h"
#include "types.hh"
#include "world.hh"

namespace Oso {

class Game {
public:
  Game();

  // Run the main loop
  void run();

  bool isPaused() const { return _paused; }
  void setPaused(bool p) { _paused = p; }

  tick_t getSimTime() const { return _simTime; }

private:
  void render();
  void updateTime(tick_t oldTime, tick_t currentTime);

  bool onCameraEvent(const SDL_Event &e);

  sdl::window _window;
  sdl::renderer _renderer;
  sdl::texture _buffer;

  tick_t _realTime = tick_t(0);
  tick_t _simTime = tick_t(0);
  bool _paused = false;
  unsigned _timeAccel = 1000;

  Camera _camera;
  World _world;
  GameGui _gui;
};

} // namespace Oso
