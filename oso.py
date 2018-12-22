#!/usr/bin/env python3

import sdl2
import sdl2.ext
import sdl2.sdlgfx
import time
import ctypes

sdl2.ext.init()

resources = sdl2.ext.Resources(__file__, "resources")

window = sdl2.ext.Window("Orbits", (640, 480))
window.show()

fm = sdl2.sdlgfx.FPSManager()
sdl2.sdlgfx.SDL_initFramerate(fm)
sdl2.sdlgfx.SDL_setFramerate(fm, 10)


renderer = sdl2.ext.Renderer(window)
# renderer.color = sdl2.ext.Color(r=255, g=255, b=255)


sprite_factory = sdl2.ext.SpriteFactory(renderer=renderer)

SPRITE_COUNT = 3
sprites = []
for i in range(SPRITE_COUNT):
    filename = 'sprite_{}.png'.format(i)
    sprite = sprite_factory.from_image(resources.get_path(filename))
    sprites.append(sprite)

#sprite_render_system = sprite_factory.create_sprite_render_system()


def render(tick):
    #renderer.draw_line([0, 0, int(tick/2), int(tick / 10)])
    renderer.clear()
    SCALE = 2
    sprite = sprites[tick % SPRITE_COUNT]
    x = 2*tick % 640
    y = 210
    angle = tick
    dstrect = (x, y, sprite.size[0]*SCALE, sprite.size[1]*SCALE)
    # sprite_render_system.render(
    #    sprites[tick % SPRITE_COUNT], x=tick % 640, y=210)
    renderer.copy(sprite, dstrect=dstrect, angle=angle)
    renderer.present()


# main loop
counter = 0
running = True
while running:
    for event in sdl2.ext.get_events():
        if event.type == sdl2.events.SDL_QUIT:
            print('QUIT!')
            running = False
            break
    counter += 1
    render(counter)
    window.refresh()
    sdl2.sdlgfx.SDL_framerateDelay(fm)
