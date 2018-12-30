#include "game.hh"

#include <SDL.h>
#include <SDL_image.h>

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

  Oso::Game game;
  game.run();

  ::SDL_Quit();
}
