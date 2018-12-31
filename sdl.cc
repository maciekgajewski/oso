#include "sdl.h"

#include <SDL_image.h>

namespace sdl {

void throw_error[[noreturn]](const std::string &why) {
  throw exception(why + " : " + ::SDL_GetError());
}

window create_window(const char *title, int x, int y, int w, int h,
                     Uint32 flags) {
  ::SDL_Window *win = ::SDL_CreateWindow(title, x, y, w, h, flags);
  if (!win)
    throw_error("Failed to create window");

  return window(win);
}

renderer create_renderer(SDL_Window *window, int index, Uint32 flags) {
  ::SDL_Renderer *r = ::SDL_CreateRenderer(window, index, flags);
  if (!r)
    throw_error("Faield to create renderer");
  return renderer(r);
}

texture create_texture(SDL_Renderer *renderer, Uint32 format, int access, int w,
                       int h) {
  ::SDL_Texture *r = ::SDL_CreateTexture(renderer, format, access, w, h);
  if (!r)
    throw_error("Faield to create texture");
  return texture(r);
}

texture create_texture_from_surface(SDL_Renderer *renderer,
                                    SDL_Surface *surface) {
  ::SDL_Texture *r = ::SDL_CreateTextureFromSurface(renderer, surface);
  if (!r)
    throw_error("Faield to create texture from surface");
  return texture(r);
}

string get_base_path() {
  char *p = SDL_GetBasePath();
  if (!p)
    throw_error("SDL_GetBasePath failed");

  return string(p);
}

surface img_load(const std::string &path) {
  SDL_Surface *s = IMG_Load(path.c_str());
  if (!s)
    throw_error("Failed to load image from " + path);
  return surface(s);
}

surface img_load_rw(::SDL_RWops *rw, bool close) {
  SDL_Surface *s = IMG_Load_RW(rw, close ? 1 : 0);
  if (!s)
    throw_error("Failed to load image from rw");
  return surface(s);
}

ttf_font open_font_rw(SDL_RWops *rw, bool close, int ptsize) {
  TTF_Font *font = TTF_OpenFontRW(rw, close ? 1 : 0, ptsize);
  if (!font) {
    throw_error("Error opening font");
  }
  return ttf_font(font);
}

} // namespace sdl
