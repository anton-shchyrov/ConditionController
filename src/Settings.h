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

class Settings {
private:
    uint8_t minTemp : MIN_TEMP_LENGTH;
    uint8_t maxTempDelta : MAX_TEMP_DELTA_LENGTH;
public:
    uint8_t getMinTemp() const;
    void setMinTemp(uint8_t val);
    uint8_t getMaxTemp() const;
    void setMaxTemp(uint8_t val);
    void load();
    void save() const;
    static void getMinTempRange(UInt8Range &range);
    void getMaxTempRange(UInt8Range &range) const;
};

#endif //CONTROLLER_SETTINGS_H

#pragma clang diagnostic pop