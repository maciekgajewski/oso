#!/usr/bin/python3

import math
import matplotlib
import matplotlib.pyplot as plt

G = 6.67408E-11
# orbit description


def findE(E0, Mt, e):
    diff = 1
    while True:
        diff = (E0 - e * math.sin(E0) - Mt) / (1 - e * math.cos(E0))
        if diff < 0.06:
            break
        E0 = E0 - diff
    return E0


class Orbit:
    def __init__(self):
        self.M0 = 0  # mean anomaly at epoch
        self.a = 0  # semi-major axis
        self.e = 0  # eccentricity
        self.w = 0  # longitude of periapsis

    def fromState(self, x, y, vx, vy, M, m):

        h = x*vy - y*vx  # momentum
        mu = (M+m)*G
        r2 = x*x + y*y
        v2 = vx*vx + vy*vy
        r = math.sqrt(r2)  # orbital separation
        ex = vy*h/mu - x/r
        ey = -vx*h/mu - y/r
        rv = x*vx + y*vy  # dot product of v and r vectors
        #ex = (v2 - mu/r)*x/mu - rv*vx/mu
        #ey = (v2 - mu/r)*y/mu - rv*vy/mu

        e2 = ex*ex + ey*ey
        self.e = math.sqrt(e2)
        E = v2/2 - mu/r
        a = -mu/(2*E)  # semi major
        p = h/mu  # semi-latus rectum
        e_alt = math.sqrt(1 - p/a)  # eccentricity, scalar

        self.a = (h*h)/mu*(1-e2)
        self.w = math.atan2(ey, ex)

    def posAtTime(self, time, M, m):
        # see:
        # https://en.wikipedia.org/wiki/True_anomaly
        # https://gamedev.stackexchange.com/questions/112335/orbital-mechanics-orbit-as-a-function-of-time-universal-variable-formulation
        # mean anomaly at time
        Mt = self.M0 + time * math.sqrt(G * (M+m) / (self.a * self.a * self.a))
        # Eccentric anomaly
        if self.e > 0.8:
            E = findE(math.pi, Mt, self.e)
        else:
            E = findE(Mt, Mt, self.e)
        # true anomaly (angle from planet)
        v = 2*math.atan2(math.sqrt(1+self.e)*math.sin(E/2),
                         math.sqrt(1-self.e)*math.cos(E/2))
        # radius (distance from planet)
        r = self.a * (1-self.e*self.e)/(1+self.e*math.cos(E))

        x = r*math.cos(v)
        y = r*math.sin(v)
        return x, y


def semiImplicitEuler(y0, x0, vx0, vy0, m, M, dt, steps):
    xs = []
    ys = []
    vys = []
    vxs = []
    x = x0
    y = y0
    vx = vx0
    vy = vy0
    mu = (M+m)*G
    for step in range(steps):
        xs.append(x)
        ys.append(y)
        vys.append(vy)
        vxs.append(vx)
        r2 = x*x + y*y
        r = math.sqrt(r2)
        a = G*(M)/r2
        ax = -a*x/r
        ay = -a*y/r
        vx += ax*dt
        vy += ay*dt
        x += vx*dt
        y += vy*dt

        # looking for invariants
        h = x*vy - y*vx  # momentum
        v2 = vx*vx + vy*vy
        Etotal = v2/2 - mu/r  # Total energy

        # eccentiricity
        ex = vy*h/mu - x/r
        ey = -vx*h/mu - y/r

        # semi-major
        a = -mu/(2*Etotal)

        print('#{}/{}\th={}, Et={}, e=({}, {}), a={}'.format(step,
                                                             steps, h, Etotal, ex, ey, a))

    return (xs, ys, vxs, vys)


def fromOrbit(orbit, m, M, dt, steps):
    xs = []
    ys = []
    for step in range(steps):
        time = step*dt
        x, y = orbit.posAtTime(time, M, m)
        xs.append(x)
        ys.append(y)
    return xs, ys


M = 5.9724E24  # mass of earth
# m = 0.07346E24  # mass of the moon
m = 0
STEPS = 26000
DT = 100
# starting at apoapsis
# xs, ys = semiImplicitEuler(y0=0, x0=0.4055E9, vx0=0, vy0=970,
#                           m = m, M = M, dt = 100, steps = 26000)

# starting at periapsis
xs, ys, vxs, vys = semiImplicitEuler(y0=0, x0=0.3633E9, vx0=0, vy0=1082,
                                     m=m, M=M, dt=DT, steps=STEPS)

# from orbit
o = Orbit()
# o.e = 0.0549 # from tables
o.e = 0.06710  # calculated
# o.a = 0.3844E9  # from tables
o.a = 0.3894E9  # calculated
o.M0 = 0

#o2 = Orbit()
#o2.fromState(x=0.3633E9, y=0, vx=0, vy=1082, M=M, m=m)

xso, yso = fromOrbit(orbit=o, dt=DT, m=m, M=M, steps=STEPS)
#xso2, yso2 = fromOrbit(orbit=o2, dt=DT, m=m, M=M, steps=STEPS)

fig, ax = plt.subplots()
ax.set(xlabel='x', ylabel='y', title='Moon orbit')
#ax.plot(xs, ys, '+', xso, yso, 'x', xso2, yso2, 'o')
ax.plot(xs, ys, '+', label='si eurler')
ax.plot(xso, yso, 'x', label='calculated')


# momentum
# _, ax2 = plt.subplots()
# hs = [x*vy - y*vx for x, y, vx, vy in zip(xs, ys, vxs, vys)]
# ax2.plot(hs, label='momentum')

plt.legend()
plt.show()
