#include "gui_control.hh"

#include "resources.hh"
#include "sdl.h"

#include <SDL_image.h>

#include <algorithm>
#include <cassert>
#include <iostream>

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

  if (e.type == SDL_MOUSEMOTION) {

    SDL_Point mouseCoords{e.motion.x, e.motion.y};

    for (Control *c : _children) {
      if (SDL_PointInRect(&mouseCoords, &c->_rect)) {
        if (_controlUnderMouse != c) {
          if (_controlUnderMouse)
            _controlUnderMouse->onMouseLeave();
          _controlUnderMouse = c;
          _controlUnderMouse->onMouseEnter();
        }
        SDL_Event copy = e;
        copy.motion.x -= c->_rect.x;
        copy.motion.y -= c->_rect.y;
        return c->onEvent(copy);
      }
    }
    if (_controlUnderMouse)
      _controlUnderMouse->onMouseLeave();
    _controlUnderMouse = nullptr;
  }

  else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
    if (_controlUnderMouse) {
      SDL_Event copy = e;
      copy.button.x -= _controlUnderMouse->_rect.x;
      copy.button.y -= _controlUnderMouse->_rect.y;
      if (_controlUnderMouse->onEvent(copy))
        return true;
    }
    return internalOnEvent(e);
  }

  else if (e.type == SDL_WINDOWEVENT) {
    if (e.window.event == SDL_WINDOWEVENT_LEAVE) {
      if (_controlUnderMouse) {
        _controlUnderMouse->onMouseLeave();
        _controlUnderMouse = nullptr;
      }
    }
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

ImgButton::ImgButton(const std::string &imgPath, const SDL_Point &pos,
                     const ImgButton::handler_t &handler)
    : _handler(handler) {

  _image = sdl::img_load_rw(open_resource(imgPath), true);
  _rect.x = pos.x;
  _rect.y = pos.y;
  _rect.h = _image->h;
  _rect.w = _image->w;
}

void ImgButton::internalRender(SDL_Renderer *renderer) {
  if (!_texture) {
    _texture = sdl::create_texture_from_surface(renderer, _image);
  }

  if (_pressed) {
    SDL_SetRenderDrawColor(renderer, 32, 32, 128, 128);
    SDL_RenderFillRect(renderer, &_rect);
  } else if (_hasMouse) {
    SDL_SetRenderDrawColor(renderer, 32, 32, 64, 128);
    SDL_RenderFillRect(renderer, &_rect);
  }

  SDL_Rect srcrect{0, 0, _rect.w, _rect.h};
  SDL_RenderCopy(renderer, _texture, &srcrect, &_rect);
}

bool ImgButton::internalOnEvent(const SDL_Event &e) {

  if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
    _pressed = true;
    return true;
  }

  if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
    if (_pressed) {
      _pressed = false;
      if (_handler)
        _handler();
      return true;
    }
  }
  return false;
}

Text::Text(const std::string &text, const SDL_Point &pos, TTF_Font *font)
    : _text(text), _font(font) {

  _rect.x = pos.x;
  _rect.y = pos.y;
}

void Text::setText(const std::string &text) { _text = text; }

void Text::internalRender(SDL_Renderer *renderer) {
  // naive rendering
  sdl::surface surf = sdl::surface(TTF_RenderUTF8_Solid(
      _font, _text.c_str(), SDL_Color{0xff, 0xff, 0x00, 0xff}));
  sdl::texture texture = sdl::create_texture_from_surface(renderer, surf);

  SDL_Rect srcrect{0, 0, surf->w, surf->h};
  _rect.w = surf->w;
  _rect.h = surf->h;
  SDL_RenderCopy(renderer, texture, &srcrect, &_rect);
}

} // namespace Gui
} // namespace Oso
