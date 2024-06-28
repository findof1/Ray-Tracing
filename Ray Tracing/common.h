#ifndef COMMON_H
#define COMMON_H

#include <limits>
#include <cmath>
#include <limits>
#include <memory>

const double infinityVal = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;


double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

double randomDouble() {
    return rand() / (RAND_MAX + 1.0);
}

double randomDouble(double min, double max) {
    return min + (max - min) * randomDouble();
}

#endif