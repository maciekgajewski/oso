#include "orbit.hh"

#include <cmath>

namespace Oso {

static double getE(double E0, double Mt, double e)
{
    double diff = 1;
    do {
        diff = (E0 - e * std::sin(E0) - Mt) / 1 - e * std::cos(E0);

        E0 = E0 - diff;
    } while (diff > 0.06);
    return E0;
}

position_t Orbit::findPosition(tick_t time, mass_t m, mass_t M) const
{
    const double G = 6.67408E-11;
    double mu = G * M;

    // mean anomaly at time
    angle_t Mt = M0 + time.count() * std::sqrt(G * (M + m) / (a * a * a));

    // accurate eccentiricity
    double E;
    if (e > 0.8)
        E = getE(M_PI, Mt, e);
    else
        E = getE(Mt, Mt, e);

    double xv = a * (std::cos(E) - e);
    double yv = a * (std::sqrt(1.0 - e * e) * std::sin(E));

    double v = std::atan2(yv, xv);
    double r = std::sqrt(xv * xv + yv * yv);

    return r * position_t{ std::cos(v), std::sin(v) };
}

} // namespace Oso
