#include "gui_control.hh"

#include "sdl.h"

#include <SDL_image.h>

#include <algorithm>
#include <cassert>

namespace Oso {
namespace Gui {

Control::Control() {
  //
}

Control::~Control() {
  if (_parent)
    _parent->removeChild(this);
}

void Control::addChild(Control *c) {
  assert(c->_parent == nullptr);
  _children.push_back(c);
  c->_parent = this;
}

void Control::addChild(std::unique_ptr<Control> child) {
  addChild(child.get());
  _ownedChildren.push_back(std::move(child));
}

void Control::removeChild(Control *c) {
  assert(c->_parent == this);

  auto it = std::find(_children.begin(), _children.end(), c);
  if (it != _children.end()) {
    _children.erase(it);
    c->_parent = nullptr;
  }
}

bool Control::onEvent(const SDL_Event &e) {
  if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
    SDL_Point mouseCoords{e.button.x, e.button.y};

    for (Control *c : _children) {
      if (SDL_PointInRect(&mouseCoords, &c->_rect)) {
        SDL_Event copy = e;
        copy.button.x -= c->_rect.x;
        copy.button.y -= c->_rect.y;
        if (c->onEvent(copy))
          return true;
      }
    }
    return internalOnEvent(e);
  }
  return false;
}

void Control::render(SDL_Renderer *renderer) {
  internalRender(renderer);
  for (Control *c : _children) {
    c->render(renderer);
  }
}

bool Control::internalOnEvent(const SDL_Event &e) { return false; }

void Control::internalRender(SDL_Renderer *renderer) {
  // nothing
}

ImgButton::ImgButton(const std::string &imgPath, const SDL_Point &pos,
                     const ImgButton::handler_t &handler) {
  sdl::string basePath = sdl::get_base_path();
  std::string fullPath = (const char *)basePath;
  fullPath += imgPath;

  _image = sdl::img_load(fullPath);
  _rect.x = pos.x;
  _rect.y = pos.y;
  _rect.h = _image->h;
  _rect.w = _image->w;
}

void ImgButton::internalRender(SDL_Renderer *renderer) {
  if (!_texture) {
    _texture = sdl::create_texture_from_surface(renderer, _image);
  }

  SDL_Rect srcrect{0, 0, _rect.w, _rect.h};
  SDL_RenderCopy(renderer, _texture, &srcrect, &_rect);
}

} // namespace Gui
} // namespace Oso
