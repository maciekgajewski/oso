#include "game_gui.hh"

#include "game.hh"
#include "resources.hh"

namespace Oso {

GameGui::GameGui(Game &game) : _game(game) {

  auto fontResource = open_resource("fonts/PlanetaryContact.ttf"); // not
  // very readable, but cool
  // auto fontResource = open_resource("fonts/pixelated.ttf"); // really small
  _font = sdl::open_font_rw(fontResource, true, 8);

  auto pauseButton =
      std::make_unique<Gui::ImgButton>("pause.png", SDL_Point{0, 0}, [&] {
        _game.setPaused(!_game.isPaused());
      });

  _simTimeText = std::make_unique<Gui::Text>(
      "00:00:00", SDL_Point{pauseButton->width(), 0}, _font);

  _main.addChild(std::move(pauseButton));
  _main.addChild(_simTimeText.get());
}

void GameGui::onEvent(SDL_Event e) {

  // re-scale all mouse events
  if (e.type == SDL_MOUSEMOTION) {
    e.motion.x /= 2;
    e.motion.y /= 2;
  } else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
    e.button.x /= 2;
    e.button.y /= 2;
  }

  _main.onEvent(e);
}

void GameGui::render(SDL_Renderer *renderer) { _main.render(renderer); }

void GameGui::update() {
  tick_t simTime = _game.getSimTime();

  std::uint64_t seconds = simTime.count() / 1000;
  std::uint64_t minutes = seconds / 60;
  std::uint64_t hours = minutes / 60;

  char text[20];
  std::sprintf(text, "%02llu:%02llu:%02llu", hours, minutes % 60, seconds % 60);
  _simTimeText->setText(text);
}

} // namespace Oso
