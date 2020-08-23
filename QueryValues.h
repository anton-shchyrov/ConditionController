//
// Created by Anton on 22.08.2020.
//

#ifndef CONTROLLER_QUERYVALUES_H
#define CONTROLLER_QUERYVALUES_H

#include "LCD1602Shield.h"

enum LoopResult {
    LR_PROCESS,
    LR_SELECT,
    LR_TIMEOUT
};

class QueryValues {
private:
    LCD1602Shield * const lcd;
    char * const chars;
    const uint8_t len;
    const uint8_t base;
    uint8_t startCol{};
    Buttons prevBtn;
    unsigned long lastTime;
private:
    uint8_t getChar(uint8_t index);
    void setChar(uint8_t index, uint8_t value);
    void incChar(uint8_t col, bool isInc);
public:
    QueryValues(LCD1602Shield & lcd, unsigned int defVal, uint8_t maxLen, uint8_t base = 10);
    ~QueryValues();
    unsigned int getCurrentVal();
    LoopResult loop();
};


#endif //CONTROLLER_QUERYVALUES_H
