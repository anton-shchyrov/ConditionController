//
// Created by Anton on 22.08.2020.
//

#include "QueryValues.h"

#define WAIT_TIME (1000 * 5)

QueryValues::QueryValues(LCD1602Shield &lcd, unsigned int defVal, uint8_t maxLen, uint8_t base) :
        lcd(&lcd), chars(new char[maxLen]), len(maxLen), base(base)
{
    this->prevBtn = LCD1602Shield::detectButton();
    this->lastTime = millis();
    for (int8_t idx = maxLen - 1; idx >= 0; idx--) {
        uint8_t cur = defVal % this->base;
        defVal /= this->base;
        setChar(idx, cur);
    }
    uint8_t row;
    this->lcd->getCursor(this->startCol, row);
    this->lcd->print(this->chars, maxLen);
    this->lcd->setCursor(this->startCol + maxLen - 1, row);
    this->lcd->showCursor(true);
}

QueryValues::~QueryValues() {
    for (int8_t i = 0; i < this->len; i++)
        this->chars[i] = ' ';
    uint8_t col, row;
    this->lcd->showCursor(false);
    this->lcd->getCursor(col, row);
    this->lcd->setCursor(this->startCol, row);
    this->lcd->print(this->chars, this->len);
    this->lcd->setCursor(this->startCol, row);
    delete [] chars;
}

uint8_t QueryValues::getChar(uint8_t index) {
    char ch = this->chars[index];
    if (ch <= '9')
        return (ch - '0');
    else
        return (ch - 'A' + 10);
}

void QueryValues::setChar(uint8_t index, uint8_t value) {
    if (value < 10)
        value += '0';
    else
        value += 'A' - 10;
    this->chars[index] = value;
}

void QueryValues::incChar(uint8_t col, bool isInc) {
    uint8_t idx = col - this->startCol;
    uint8_t cur = getChar(idx);
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
    setChar(idx, cur);
    this->lcd->print(&this->chars[idx], 1);
}

unsigned int QueryValues::getCurrentVal() {
    int res = 0;
    for (int8_t i = 0; i < this->len; i++)
        res = res * this->base + getChar(i);
    return res;
}

LoopResult QueryValues::loop() {
    unsigned long curTime = millis();
    const unsigned long delta = curTime - this->lastTime;
    if (delta > WAIT_TIME)
        return LR_TIMEOUT;
    Buttons curBtn = LCD1602Shield::detectButton();
    if (curBtn == this->prevBtn)
        return LR_PROCESS;
    this->prevBtn = curBtn;
    this->lastTime = curTime;
    if (curBtn == BTN_SELECT)
        return LR_SELECT;
    uint8_t col, row;
    this->lcd->getCursor(col, row);
    switch (curBtn) {
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
    this->lcd->setCursor(col, row);
    return LR_PROCESS;
}
