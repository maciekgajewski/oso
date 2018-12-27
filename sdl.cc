#include "sdl.h"

namespace sdl {

void throw_error[[noreturn]](const std::string& why)
{
    throw exception(why + " : " + ::SDL_GetError());
}

window create_window(const char* title,
    int x,
    int y,
    int w,
    int h,
    Uint32 flags)
{
    ::SDL_Window* win = ::SDL_CreateWindow(title, x, y, w, h, flags);
    if (!win)
        throw_error("Failed to create window");

    return window(win);
}

renderer create_renderer(SDL_Window* window, int index, Uint32 flags)
{
    ::SDL_Renderer* r = ::SDL_CreateRenderer(window, index, flags);
    if (!r)
        throw_error("Faield to create renderer");
    return renderer(r);
}

texture create_texture(SDL_Renderer* renderer, Uint32 format, int access, int w, int h)
{
    ::SDL_Texture* r = ::SDL_CreateTexture(renderer, format, access, w, h);
    if (!r)
        throw_error("Faield to create texture");
    return texture(r);
}

} // sdl
