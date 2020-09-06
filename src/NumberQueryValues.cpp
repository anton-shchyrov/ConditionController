//
// Created by Anton on 06.09.2020.
//

#include "NumberQueryValues.h"
#include "DigitQueryValues.h"

#define REFRESH_TIME 500

NumberQueryValues::NumberQueryValues(
        LCD1602Shield &lcd,
        query_t defVal,
        const Range<query_t> &range,
        uint8_t base
) : CustomQueryValues(lcd, defVal, range, base),
    lastShowTime(millis()),
    visible(true)
{}

void NumberQueryValues::incValue(bool isInc) {
    Serial.print(String("Old: ") + this->value);
    if (isInc)
        this->value++;
    else
        this->value--;
    Serial.print(String(", New: ") + this->value);
    if (this->value < this->range.min)
        this->value = this->range.max;
    else if (this->value > this->range.max)
        this->value = this->range.min;
    Serial.println(String(", Res: ") + this->value);
    if (this->visible)
        this->printValue();
}

void NumberQueryValues::doBeforeLoop() {
    CustomQueryValues::doBeforeLoop();
    unsigned long curTime = millis();
    if (curTime - this->lastShowTime > REFRESH_TIME) {
        uint8_t r, c;
        this->lcd->getCursor(c, r);
        this->lcd->setCursor(this->startCol, r);
        if (this->visible) {
            this->hideValue();
        } else
            this->printValue();
        this->visible = !this->visible;
        this->lcd->setCursor(c, r);
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
