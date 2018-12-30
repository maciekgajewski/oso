#include "camera.hh"

namespace Oso {

Camera::Camera(int screenW, int screenH)
    : _screenW(screenW)
    , _screenH(screenH)
{
}

SDL_Point Camera::physToScreen(const position_t& pos) const
{
    SDL_Point res;

    res.x = int(_screenW / 2 + pos.x * _scale);
    res.y = int(_screenH / 2 - pos.y * _scale);

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
