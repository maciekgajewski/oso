#pragma once

#include "types.hh"

#include <SDL_render.h>

#include <vector>

namespace Oso {

class Camera;

class Planet {
public:
    Planet();

    void generate(length_t avgHeight, length_t variance);

    void render(const Camera& cam, SDL_Renderer* renderer);

private:
    struct point_t {
        angle_t l;
        length_t h;
    };

    std::vector<point_t> _heightMap;
};

} // namespace Oso
