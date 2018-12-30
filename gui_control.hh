#pragma once

#include "sdl.h"

#include <functional>
#include <vector>

namespace Oso {
namespace Gui {

class Control {
public:
  Control();
  virtual ~Control();

  void addChild(Control *);
  void addChild(std::unique_ptr<Control> child);
  void removeChild(Control *);

  bool onEvent(const SDL_Event &e);
  void render(SDL_Renderer *renderer);

protected:
  virtual bool internalOnEvent(const SDL_Event &e);
  virtual void internalRender(SDL_Renderer *renderer) {}

  void onMouseEnter() {
    _hasMouse = true;
    internalOnMouseEnter();
  }
  void onMouseLeave() {
    _hasMouse = false;
    internalOnMouseLeave();
  }

  virtual void internalOnMouseEnter() {}
  virtual void internalOnMouseLeave() {}

  SDL_Rect _rect = {0, 0, 0, 0};
  bool _hasMouse = false;

private:
  Control *_controlUnderMouse = nullptr;
  Control *_parent = nullptr;
  std::vector<Control *> _children;
  std::vector<std::unique_ptr<Control>> _ownedChildren;
};

class ImgButton : public Control {
public:
  using handler_t = std::function<void()>;
  ImgButton(const std::string &imgPath, const SDL_Point &pos,
            const handler_t &handler);

private:
  void internalRender(SDL_Renderer *renderer) override;
  bool internalOnEvent(const SDL_Event &e) override;

  void internalOnMouseLeave() override { _pressed = false; }

  bool _pressed = false;

  handler_t _handler;
  sdl::surface _image;
  sdl::texture _texture;
};

} // namespace Gui
} // namespace Oso
