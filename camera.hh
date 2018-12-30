#pragma once

#include "types.hh"

#include <SDL.h>

namespace Oso {

class Camera {
public:
    Camera(int screenW, int screenH);

    SDL_Point physToScreen(const position_t& pos) const;

    void setZoomLevel(int zoom);

private:
    int _screenW;
    int _screenH;
    angle_t _angle = 0.0;
    int _zoom = 0;

    double _scale = 1.0;
};

} // namespace Oso
