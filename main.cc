#include "game.hh"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>

int main(int, char **) {
  if (::SDL_Init(SDL_INIT_VIDEO) != 0) {
    ::SDL_Log("Unable to initialize SDL: %s", ::SDL_GetError());
    return 1;
  }

  if (::IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
    ::SDL_Log("Unable to initialize SDL_image: %s", IMG_GetError());
    return 2;
  }

  if (::TTF_Init()) {
    ::SDL_Log("Unable to initialize SDL_ttf: %s", TTF_GetError());
    return 3;
  }

  Oso::Game game;
  game.run();

  ::SDL_Quit();
}
