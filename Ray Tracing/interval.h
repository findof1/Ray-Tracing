#ifndef INTERVAL_H
#define INTERVAL_H

#include "common.h"

class interval {
public:
    double min, max;
    
    interval() : min(+infinityVal), max(-infinityVal){}

    interval(double min, double max) : min(min), max(max){}

    double size() {
        return max - min;
    }

    bool contains(double value) {
        return min <= value && value <= max;
    }

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    bool surrounds(double value) {
        return min < value && value < max;
    }

    static const interval empty, universe;
};

const interval interval::empty = interval(+infinityVal, -infinityVal);
const interval interval::universe = interval(-infinityVal, +infinityVal);

#endif