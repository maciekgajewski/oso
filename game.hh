#pragma once

#include "camera.hh"
#include "sdl.h"
#include "types.hh"
#include "world.hh"

#include "gui_control.hh"

namespace Oso {

class Game {
public:
  Game();

  // Run the main loop
  void run();

private:
  void initGui();

  void render();
  void updateTime(tick_t oldTime, tick_t currentTime);
  void sendEventToGui(::SDL_Event &e);
  sdl::window _window;
  sdl::renderer _renderer;
  sdl::texture _buffer;

  tick_t _realTime = tick_t(0);
  tick_t _simTime = tick_t(0);

  Camera _camera;
  World _world;
  Gui::Control _gui;
};

} // namespace Oso
