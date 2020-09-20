//
// Created by Anton on 06.09.2020.
//

#include "DigitQueryValues.h"
#include "globals.h"

DigitQueryValues::DigitQueryValues(
        query_t defVal,
        const Range<query_t> &range,
        uint8_t base
) : DigitQueryValues(defVal, range, nullptr, base) {}

DigitQueryValues::DigitQueryValues(
        query_t defVal,
        const Range<query_t> &range,
        const char * suffix,
        uint8_t base
) : CustomQueryValues(defVal, range, suffix, base) {
    lcd.showCursor(true);
}

DigitQueryValues::~DigitQueryValues() {
    lcd.showCursor(false);
}

void DigitQueryValues::incChar(uint8_t col, bool isInc) {
    int8_t idx = this->len - (col - this->startCol) - 1;
    query_t mask = 1;
    for (int8_t i = 0; i < idx; i++)
        mask *= this->base;
    query_t low = this->value % mask;
    query_t high = this->value / mask;
    uint8_t cur = high % this->base;
    high -= cur;
    if (isInc) {
        if (cur == this->base - 1)
            cur = 0;
        else
            cur++;
    } else {
        if (cur == 0)
            cur = this->base - 1;
        else
            cur--;
    }
    this->value = (high + cur) * mask + low;
    this->range.normalizeValue(this->value);
    this->printValue();
}

void DigitQueryValues::doPressButton(Buttons btn) {
    uint8_t col, row;
    lcd.getCursor(col, row);
    switch (btn) {
        case BTN_LEFT:
            if (col <= this->startCol)
                col = this->startCol + this->len - 1;
            else
                col--;
            break;
        case BTN_RIGHT:
            if (col >= this->startCol + this->len - 1)
                col = this->startCol;
            else
                col++;
            break;
        case BTN_DOWN:
            this->incChar(col, false);
            break;
        case BTN_UP:
            this->incChar(col, true);
            break;
    }
    lcd.setCursor(col, row);
}
