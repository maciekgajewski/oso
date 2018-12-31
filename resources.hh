#pragma once

#include <SDL.h>

#include <string>

namespace Oso {

SDL_RWops *open_resource(const std::string &path);

} // namespace Oso
