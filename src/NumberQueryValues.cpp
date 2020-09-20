//
// Created by Anton on 06.09.2020.
//

#include "NumberQueryValues.h"
#include "globals.h"

#define REFRESH_TIME 500

NumberQueryValues::NumberQueryValues(
        query_t defVal,
        const Range<query_t> &range,
        uint8_t base
) : NumberQueryValues(defVal, range, nullptr, base)
{}

NumberQueryValues::NumberQueryValues(
        query_t defVal,
        const Range<query_t> &range,
        const char * suffix,
        uint8_t base
) : CustomQueryValues(defVal, range, suffix, base),
    lastShowTime(millis()),
    visible(true)
{}

void NumberQueryValues::incValue(bool isInc) {
    if (isInc)
        this->value++;
    else
        this->value--;
    if (this->value < this->range.min)
        this->value = this->range.max;
    else if (this->value > this->range.max)
        this->value = this->range.min;
    if (this->visible)
        this->printValue();
}

void NumberQueryValues::doBeforeLoop() {
    CustomQueryValues::doBeforeLoop();
    unsigned long curTime = millis();
    if (curTime - this->lastShowTime > REFRESH_TIME) {
        uint8_t r, c;
        lcd.getCursor(c, r);
        lcd.setCursor(this->startCol, r);
        if (this->visible) {
            this->hideValue();
        } else
            this->printValue();
        this->visible = !this->visible;
        lcd.setCursor(c, r);
        this->lastShowTime = curTime;
    }
}

void NumberQueryValues::doPressButton(Buttons btn) {
    switch (btn) {
        case BTN_UP:
            this->incValue(true);
            break;
        case BTN_DOWN:
            this->incValue(false);
            break;
    }
}
