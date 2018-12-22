#!/usr/bin/env python3

import sdl2
import sdl2.ext
import sdl2.sdlgfx
import time
import ctypes

sdl2.ext.init()

window = sdl2.ext.Window("Orbits", (640, 480))
window.show()

fm = sdl2.sdlgfx.FPSManager()
sdl2.sdlgfx.SDL_initFramerate(fm)
sdl2.sdlgfx.SDL_setFramerate(fm, 10)

# main loop
counter = 0
running = True
while running:
    #event = sdl2.events.SDL_Event()
    for event in sdl2.ext.get_events():
        #ret = sdl2.events.SDL_PollEvent(ctypes.byref(event), 1)
        # if ret == 1:
        print('event type: {:x}'.format(event.type))
        if event.type == sdl2.events.SDL_QUIT:
            print('QUIT!')
            running = False
            break
    window.refresh()
    print('loop {}'.format(counter))
    counter += 1
    sdl2.sdlgfx.SDL_framerateDelay(fm)

#loop = sdl2.ext.TestEventProcessor()
# loop.run(window)
