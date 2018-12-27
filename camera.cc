#include "camera.hh"

namespace Oso {

Camera::Camera(int screenW, int screenH)
    : _screenW(screenW)
    , _screenH(screenH)
{
}

SDL_Point Camera::physToScreen(double x, double y) const
{
    SDL_Point res;

    res.x = int(_screenW / 2 + x * _scale);
    res.y = int(_screenH / 2 - y * _scale);

    return res;
}

void Camera::setZoomLevel(int zoom)
{
    if (zoom > 0) {
        _scale = 1 << zoom;
    } else if (zoom < 0) {
        _scale = 1.0 / (1 << (-zoom));
    } else
        _scale = 1.0;
}

} // namespace Oso
