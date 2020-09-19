//
// Created by Anton on 24.08.2020.
//

#include "Settings.h"

#define MAX_VALUE(size) ((1u << size) - 1)

#define BASE_TEMP 18
#define MAX_MIN_TEMP (BASE_TEMP + MAX_VALUE(MIN_TEMP_LENGTH))
#define MAX_DELTA MAX_VALUE(MAX_TEMP_DELTA_LENGTH)

Settings::Settings() {}

temp_t Settings::getMinTemp() const {
    return this->minTemp + BASE_TEMP;
}

void Settings::setMinTemp(temp_t val) {
    temp_t maxTemp = this->getMaxTemp();
    if (val < BASE_TEMP)
        val = BASE_TEMP;
    else if (val > MAX_MIN_TEMP)
        val = MAX_MIN_TEMP;
    this->minTemp = val - BASE_TEMP;
    this->setMaxTemp(maxTemp);
}

temp_t Settings::getMaxTemp() const {
    return this->getMinTemp() + this->maxTempDelta + 1;
}

void Settings::setMaxTemp(temp_t val) {
    temp_t minTemp = this->getMinTemp();
    if (val <= minTemp)
        val = minTemp + 1;
    else if (val > minTemp + MAX_DELTA)
        val = minTemp + MAX_DELTA;
    this->maxTempDelta = val - minTemp - 1;
}

void Settings::load() {
    this->setMinTemp(25);
}

void Settings::save() const {

}

void Settings::getMinTempRange(Range<temp_t> &range) {
    range.min = BASE_TEMP;
    range.max = MAX_MIN_TEMP;
}

void Settings::getMaxTempRange(Range<temp_t> &range) const {
    range.min = this->getMinTemp() + 1;
    range.max = range.min + MAX_DELTA;
}
