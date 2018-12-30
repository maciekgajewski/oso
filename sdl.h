#pragma once

#include <SDL.h>

#include <memory>
#include <stdexcept>
#include <string>

namespace sdl {

class exception : public std::exception {
public:
  exception(const std::string &e) : _e(e) {}

  const char *what() const noexcept override { return _e.c_str(); }

private:
  std::string _e;
};

void throw_error[[noreturn]](const std::string &why);

template <typename T, void(DeleterFun)(T *)> struct UniquePtr {
  UniquePtr() = default;
  UniquePtr(T *p) : _p(p) {}
  UniquePtr(const UniquePtr &) = delete;
  UniquePtr(UniquePtr &&o) {
    _p = o._p;
    o._p = nullptr;
  }
  ~UniquePtr() {
    if (_p)
      DeleterFun(_p);
  }

  UniquePtr &operator=(const UniquePtr &o) = delete;
  UniquePtr &operator=(UniquePtr &&o) {
    if (_p)
      DeleterFun(_p);
    _p = o._p;
    o._p = nullptr;
    return *this;
  }

  operator T *() { return _p; }
  T *operator->() { return _p; }

private:
  T *_p = nullptr;
};

using window = UniquePtr<::SDL_Window, &::SDL_DestroyWindow>;
window create_window(const char *title, int x, int y, int w, int h,
                     Uint32 flags);

using renderer = UniquePtr<::SDL_Renderer, &::SDL_DestroyRenderer>;
renderer create_renderer(SDL_Window *window, int index, Uint32 flags);

using texture = UniquePtr<::SDL_Texture, &::SDL_DestroyTexture>;
texture create_texture(SDL_Renderer *renderer, Uint32 format, int access, int w,
                       int h);

using surface = UniquePtr<::SDL_Surface, &::SDL_FreeSurface>;
surface img_load(const std::string &path);
texture create_texture_from_surface(SDL_Renderer *renderer,
                                    SDL_Surface *surface);

inline void free_char(char *c) { ::SDL_free(c); }

using string = UniquePtr<char, &free_char>;
string get_base_path();

} // namespace sdl
