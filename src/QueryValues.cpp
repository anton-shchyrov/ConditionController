//
// Created by Anton on 22.08.2020.
//

#include "QueryValues.h"

#define MAX_LEN 16
#define WAIT_TIME (1000 * 5)
#define REFRESH_TIME 500

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

QueryValues::QueryValues(LCD1602Shield &lcd, query_t defVal, const Range<query_t> &range, uint8_t base) :
        lcd(&lcd),
        range(range),
        base(base),
        len(MIN(this->getNumCount(range.max), MAX_LEN))
{
    this->value = defVal;
    this->prevBtn = LCD1602Shield::detectButton();
    this->lastPressBtnTime = millis();
    this->lastShowTime = this->lastPressBtnTime;
    this->visible = true;
    uint8_t row;
    this->lcd->getCursor(this->startCol, row);
    this->printValue();
    this->lcd->setCursor(this->startCol + this->len - 1, row);
    this->lcd->showCursor(true);
}

QueryValues::~QueryValues() {
    this->hideValue();
    this->lcd->showCursor(false);
    uint8_t col, row;
    this->lcd->getCursor(col, row);
    this->lcd->setCursor(this->startCol, row);
}

uint8_t QueryValues::getNumCount(query_t val) const {
    uint8_t res = 1;
    while (val = val / this->base)
        res++;
    return res;
}

void QueryValues::printBuffer(char *buf, uint8_t len) {
    uint8_t col, row;
    this->lcd->getCursor(col, row);
    this->lcd->setCursor(this->startCol, row);
    this->lcd->print(buf, len);
    this->lcd->setCursor(col, row);
}

void QueryValues::printValue() {
    char buf[MAX_LEN];
    uint8_t len = MAX_LEN;
    char * data = valToChars(buf, len, this->value);
    this->printBuffer(data, len);
}

void QueryValues::hideValue() {
    char buf[MAX_LEN];
    uint8_t len = MIN(MAX_LEN, this->len);
    memset(buf, ' ', len);
    this->printBuffer(buf, len);
}

char * QueryValues::valToChars(char buf[], uint8_t & len, query_t val) {
    len = MIN(len, this->len);
    for (int8_t idx = len - 1; idx >= 0; idx--) {
        uint8_t cur = val % this->base;
        val /= this->base;
        buf[idx] = encodeChar(cur);
    }
    return buf;
}

char QueryValues::encodeChar(uint8_t num) {
    if (num < 10)
        num += '0';
    else
        num += 'A' - 10;
    return num;
}

uint8_t QueryValues::decodeChar(char ch) {
    if (ch <= '9')
        return (ch - '0');
    else
        return (ch - 'A' + 10);
}

void QueryValues::incChar(uint8_t col, bool isInc) {
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
    if (this->value < this->range.min)
        this->value = this->range.min;
    else if (this->value > this->range.max)
        this->value = this->range.max;
    this->printValue();
}

query_t QueryValues::getCurrentVal() const {
    return this->value;
}

LoopResult QueryValues::loop() {
    unsigned long curTime = millis();
/*    if (curTime - this->lastShowTime > REFRESH_TIME) {
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
*/    if (curTime - this->lastPressBtnTime > WAIT_TIME)
        return LR_TIMEOUT;
    Buttons curBtn = LCD1602Shield::detectButton();
    if (curBtn == this->prevBtn)
        return LR_PROCESS;
    this->prevBtn = curBtn;
    this->lastPressBtnTime = curTime;
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
