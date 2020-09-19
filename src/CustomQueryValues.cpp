//
// Created by Anton on 22.08.2020.
//

#include "CustomQueryValues.h"
#include "globals.h"

#define MAX_LEN 16
#define WAIT_TIME (1000 * 5)

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

CustomQueryValues::CustomQueryValues(
        query_t defVal,
        const Range<query_t> &range,
        uint8_t base
) :
        range(range),
        base(base),
        len(MIN(this->getNumCount(range.max), MAX_LEN)),
        startCol(this->getCurCol())
{
    this->value = defVal;
    this->prevBtn = LCD1602Shield::detectButton();
    this->lastPressBtnTime = millis();
    this->printValue();
    uint8_t col, row;
    lcd.getCursor(col, row);
    lcd.setCursor(this->startCol + this->len - 1, row);
}

CustomQueryValues::~CustomQueryValues() {
    this->hideValue();
    uint8_t col, row;
    lcd.getCursor(col, row);
    lcd.setCursor(this->startCol, row);
}

uint8_t CustomQueryValues::getNumCount(query_t val) const {
    uint8_t res = 1;
    while (val = val / this->base)
        res++;
    return res;
}

uint8_t CustomQueryValues::getCurCol() const {
    uint8_t col, row;
    lcd.getCursor(col, row);
    return col;
}


void CustomQueryValues::printBuffer(char *buf, uint8_t len) const {
    uint8_t col, row;
    lcd.getCursor(col, row);
    lcd.setCursor(this->startCol, row);
    lcd.print(buf, len);
    lcd.setCursor(col, row);
}

char * CustomQueryValues::valToChars(char buf[], uint8_t & len, query_t val) {
    len = MIN(len, this->len);
    for (int8_t idx = len - 1; idx >= 0; idx--) {
        uint8_t cur = val % this->base;
        val /= this->base;
        buf[idx] = encodeChar(cur);
    }
    return buf;
}

char CustomQueryValues::encodeChar(uint8_t num) {
    if (num < 10)
        num += '0';
    else
        num += 'A' - 10;
    return num;
}

uint8_t CustomQueryValues::decodeChar(char ch) {
    if (ch <= '9')
        return (ch - '0');
    else
        return (ch - 'A' + 10);
}

void CustomQueryValues::printValue() {
    char buf[MAX_LEN];
    uint8_t len = MAX_LEN;
    char * data = valToChars(buf, len, this->value);
    this->printBuffer(data, len);
}

void CustomQueryValues::hideValue() {
    char buf[MAX_LEN];
    uint8_t len = MIN(MAX_LEN, this->len);
    memset(buf, ' ', len);
    this->printBuffer(buf, len);
}

void CustomQueryValues::doBeforeLoop() {
    // Empty
}

query_t CustomQueryValues::getCurrentVal() const {
    return this->value;
}

LoopResult CustomQueryValues::loop() {
    this->doBeforeLoop();
    unsigned long curTime = millis();
    if (curTime - this->lastPressBtnTime > WAIT_TIME)
        return LR_TIMEOUT;
    Buttons curBtn = LCD1602Shield::detectButton();
    if (curBtn == this->prevBtn)
        return LR_PROCESS;
    this->prevBtn = curBtn;
    this->lastPressBtnTime = curTime;
    if (curBtn == BTN_SELECT)
        return LR_SELECT;
    this->doPressButton(curBtn);
    return LR_PROCESS;
}
