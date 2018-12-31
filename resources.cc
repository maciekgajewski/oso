#include "resources.hh"

#include "sdl.h"

namespace Oso {

class Resources {
public:
  Resources() {
    sdl::string basePath = sdl::get_base_path();
    _base = (const char *)basePath;
    _base += "resources/";
  }

  SDL_RWops *open_resource(const std::string &name) {
    std::string fp = fullPath(name);
    SDL_RWops *res = SDL_RWFromFile(fp.c_str(), "r");
    if (!res)
      throw std::runtime_error("Error opening resource: " + name);
    return res;
  }

private:
  std::string fullPath(const std::string &name) { return _base + name; }

  std::string _base;
};

static Resources &getResources() {
  static Resources r;
  return r;
}

SDL_RWops *open_resource(const std::string &name) {
  return getResources().open_resource(name);
}

} // namespace Oso
