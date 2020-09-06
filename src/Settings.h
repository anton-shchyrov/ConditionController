#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-deprecated-headers"
//
// Created by Anton on 24.08.2020.
//

#ifndef CONTROLLER_SETTINGS_H
#define CONTROLLER_SETTINGS_H

#include <stdint.h>
#include "types.h"

#define MIN_TEMP_LENGTH 3u
#define MAX_TEMP_DELTA_LENGTH 3u

#define temp_t uint8_t

class Settings {
private:
    temp_t minTemp : MIN_TEMP_LENGTH;
    temp_t maxTempDelta : MAX_TEMP_DELTA_LENGTH;
public:
    temp_t getMinTemp() const;
    void setMinTemp(temp_t val);
    temp_t getMaxTemp() const;
    void setMaxTemp(temp_t val);
    void load();
    void save() const;
    static void getMinTempRange(Range<temp_t> &range);
    void getMaxTempRange(Range<temp_t> &range) const;
};

#endif //CONTROLLER_SETTINGS_H

#pragma clang diagnostic pop