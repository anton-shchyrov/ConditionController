#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-deprecated-headers"
//
// Created by Anton on 24.08.2020.
//

#ifndef CONTROLLER_TYPES_H
#define CONTROLLER_TYPES_H

#include <stdint.h>

template <typename T> struct Range {
    T min;
    T max;
    void normalizeValue(T & val) const {
        if (val < this->min)
            val = this->min;
        else if (val > this->max)
            val = this->max;
    }
};

#endif //CONTROLLER_TYPES_H

#pragma clang diagnostic pop