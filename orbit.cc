#include "orbit.hh"

#include <cmath>

namespace Oso {

static double getE(double E0, double Mt, double e) {
  double diff = 1;
  do {
    diff = (E0 - e * std::sin(E0) - Mt) / 1 - e * std::cos(E0);

    E0 = E0 - diff;
  } while (diff > 0.06);
  return E0;
}

position_t Orbit::findPosition(tick_t time, mass_t m, mass_t M) const {
  double mu = G * M;

  // mean anomaly at time
  angle_t Mt = M0 + time.count() * 0.001 * std::sqrt(mu / (a * a * a));

  // accurate eccentiricity
  double E;
  if (e > 0.8)
    E = getE(M_PI, Mt, e);
  else
    E = getE(Mt, Mt, e);

  // true anomaly (angle from planet)
  angle_t v = 2 * std::atan2(std::sqrt(1 + e) * std::sin(E / 2),
                             std::sqrt(1 - e) * std::cos(E / 2));

  // radius (distance from planet)
  distnce_t r = a * (1 - e * e) / (1 + e * std::cos(E));

  // what is this?
  // double xv = a * (std::cos(E) - e);
  // double yv = a * (std::sqrt(1.0 - e * e) * std::sin(E));

  // double v = std::atan2(yv, xv);
  // double r = std::sqrt(xv * xv + yv * yv);

  return r * position_t{std::cos(v), std::sin(v)};
}

Orbit Orbit::circular(distnce_t radius, angle_t M0) {
  Orbit o;
  o.M0 = M0;
  o.a = radius;
  o.e = 0;
  o.w = 0;

  return o;
}

} // namespace Oso
