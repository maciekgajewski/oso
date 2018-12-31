#pragma once

#include "gui_control.hh"

namespace Oso {

class Game;

class GameGui {
public:
  GameGui(Game &game);

  void onEvent(SDL_Event e);
  void render(SDL_Renderer *renderer);
  void update();

private:
  Game &_game;
  Gui::Control _main;
  sdl::ttf_font _font;
  std::unique_ptr<Gui::Text> _simTimeText;
};

} // namespace Oso
