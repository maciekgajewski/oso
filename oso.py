#!/usr/bin/env python3

import sdl2
import sdl2.ext
import sdl2.sdlgfx
import time
import ctypes
import math

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


# sprite_render_system = sprite_factory.create_sprite_render_system()

world = sdl2.ext.World()
world.time = 0
world.prev_time = 0


class Position:
    def __init__(self, x: float, y: float, a: float)->None:
        self.x = x
        self.y = y
        self.a = a


class Physical:
    def __init__(self, mass: float, angular_mass: float, vx: float = 0, vy: float = 0, va: float = 0)->None:
        self.mass = mass
        self.angular_mass = angular_mass
        self.vx = vx
        self.vy = vy
        self.va = va
        self.ax = 0
        self.ay = 0
        self.aa = 0


class Rocket(sdl2.ext.Entity):
    def __init__(self, world: sdl2.ext.World, inital_pos: Position):
        self.sprite = None
        self.position = inital_pos
        self.physical = Physical(1.0, 1.0)


class RocketController:

    def __init__(self, rocket: Rocket):
        self.rocket = rocket
        self.__base_sprite = sprite_factory.from_image(
            resources.get_path('rocket.png'))
        self.__engine_on_sprites = []
        for i in range(3):
            filename = 'rocket_{}.png'.format(i)
            sprite = sprite_factory.from_image(resources.get_path(filename))
            self.__engine_on_sprites.append(sprite)

        self.rocket.sprite = self.__base_sprite
        self.engine_on = False
        self.ROTATION = 80
        self.ACCEL = 20

    def set_left(self, on: bool):
        self.rocket.physical.va = -self.ROTATION if on else 0

    def set_right(self, on: bool):
        self.rocket.physical.va = +self.ROTATION if on else 0

    def set_forward(self, on: bool):
        self.engine_on = on

    def process(self):
        if self.engine_on:
            frame = int(world.time/200) % 3
            self.rocket.sprite = self.__engine_on_sprites[frame]
            self.rocket.physical.ax = self.ACCEL * \
                math.cos(self.rocket.position.a*0.017453)
            self.rocket.physical.ay = self.ACCEL * \
                math.sin(self.rocket.position.a*0.017453)
        else:
            self.rocket.sprite = self.__base_sprite
            self.rocket.physical.ax = 0
            self.rocket.physical.ay = 0

        self.rocket.physical.ay += 10


rocket = Rocket(world, Position(320, 200, -90))
rocket_controller = RocketController(rocket)


# def render(tick):
#     #renderer.draw_line([0, 0, int(tick/2), int(tick / 10)])
#     renderer.clear()
#     SCALE = 2
#     sprite = sprites[tick % SPRITE_COUNT]
#     x = 2*tick % 640
#     y = 210
#     angle = tick
#     dstrect = (x, y, sprite.size[0]*SCALE, sprite.size[1]*SCALE)
#     # sprite_render_system.render(
#     #    sprites[tick % SPRITE_COUNT], x=tick % 640, y=210)
#     renderer.copy(sprite, dstrect=dstrect, angle=angle)
#     renderer.present()


class RenderingSystem(sdl2.ext.Applicator):

    def __init__(self):
        super().__init__()
        self.componenttypes = Position, sdl2.ext.Sprite

    def process(self, world, componentsets):

        SCALE = 2

        renderer.clear()
        for position, sprite in componentsets:
            dstrect = (int(position.x), int(position.y),
                       sprite.size[0]*SCALE, sprite.size[1]*SCALE)
            renderer.copy(sprite, dstrect=dstrect, angle=position.a)

        renderer.present()


class PhysicSystem(sdl2.ext.Applicator):

    def __init__(self):
        super().__init__()
        self.componenttypes = Position, Physical

    def process(self, world, componentsets):

        dt = (world.time - world.prev_time)/1000.0
        for position, physical in componentsets:
            physical.vx += physical.ax*dt
            physical.vy += physical.ay*dt
            physical.va += physical.aa*dt

            position.x += physical.vx*dt
            position.y += physical.vy*dt
            position.a += physical.va*dt


world.add_system(PhysicSystem())
world.add_system(RenderingSystem())

# main loop
counter = 0
running = True
while running:
    for event in sdl2.ext.get_events():
        if event.type == sdl2.events.SDL_QUIT:
            print('QUIT!')
            running = False
            break

        elif event.type == sdl2.SDL_KEYDOWN:
            if event.key.keysym.sym == sdl2.SDLK_UP:
                rocket_controller.set_forward(True)

            elif event.key.keysym.sym == sdl2.SDLK_LEFT:
                rocket_controller.set_left(True)

            elif event.key.keysym.sym == sdl2.SDLK_RIGHT:
                rocket_controller.set_right(True)

        elif event.type == sdl2.SDL_KEYUP:
            if event.key.keysym.sym == sdl2.SDLK_UP:
                rocket_controller.set_forward(False)

            elif event.key.keysym.sym == sdl2.SDLK_LEFT:
                rocket_controller.set_left(False)

            elif event.key.keysym.sym == sdl2.SDLK_RIGHT:
                rocket_controller.set_right(False)

    counter += 1

    rocket_controller.process()
    world.process()
    dt = sdl2.sdlgfx.SDL_framerateDelay(fm)
    world.prev_time = world.time
    world.time += dt
