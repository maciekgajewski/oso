#include "planet.hh"

#include "camera.hh"

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

void Planet::render(const Camera& cam, SDL_Renderer* renderer)
{
    std::vector<SDL_Point> points;
    points.reserve(_heightMap.size() + 1);

    std::transform(_heightMap.begin(), _heightMap.end(), std::back_inserter(points),
        [&](const point_t& p) {
            double x = p.h * std::sin(p.l * M_PI / 180.0);
            double y = p.h * std::cos(p.l * M_PI / 180.0);
            return cam.physToScreen(x, y);
        });
    points.push_back(points[0]);

    SDL_SetRenderDrawColor(renderer, 150, 150, 150, SDL_ALPHA_OPAQUE);
    ::SDL_RenderDrawLines(renderer, points.data(), points.size());
}

} // namespace Oso
