#include "game.hh"

#include <SDL.h>

#include <iostream>

int main(int, char**)
{
    if (::SDL_Init(SDL_INIT_VIDEO) != 0) {
        ::SDL_Log("Unable to initialize SDL: %s", ::SDL_GetError());
        return 1;
    }

    Oso::Game game;
    game.run();

    ::SDL_Quit();
}
