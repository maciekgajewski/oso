#include "planet.hh"

#include "camera.hh"

#include <SDL2_gfxPrimitives.h>

#include <algorithm>
#include <cmath>
#include <iterator>
#include <random>

namespace Oso {

Planet::Planet()
{
}

void Planet::generate(length_t avgHeight, length_t variance)
{
    std::uniform_real_distribution<length_t> dist(avgHeight - variance, avgHeight + variance);
    std::random_device rd;

    unsigned segments = 20;

    _heightMap.clear();
    _heightMap.reserve(segments);

    for (unsigned i = 0; i < segments; ++i) {
        point_t p;
        p.l = i * 360.0 / segments;
        p.h = dist(rd);

        _heightMap.push_back(p);
    }
}

void Planet::render(const Camera& cam, SDL_Renderer* renderer, const location_t& pos) const
{
    std::vector<Sint16> xs;
    std::vector<Sint16> ys;
    xs.reserve(_heightMap.size());
    ys.reserve(_heightMap.size());

    for (unsigned i = 0; i < _heightMap.size(); i++) {
        const point_t& p = _heightMap[i];
        position_t phys = p.h * position_t{ std::sin((p.l + pos.a) * M_PI / 180.0), std::cos((p.l + pos.a) * M_PI / 180.0) } + pos.pos;
        SDL_Point point = cam.physToScreen(phys);
        xs.push_back(point.x);
        ys.push_back(point.y);
    }
    ::filledPolygonRGBA(renderer, xs.data(), ys.data(), _heightMap.size(), 150, 150, 150, SDL_ALPHA_OPAQUE);
}

} // namespace Oso
