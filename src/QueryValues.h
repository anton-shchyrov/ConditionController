//
// Created by Anton on 22.08.2020.
//

#ifndef CONTROLLER_QUERYVALUES_H
#define CONTROLLER_QUERYVALUES_H

#include "types.h"
#include "LCD1602Shield.h"

enum LoopResult {
    LR_PROCESS,
    LR_SELECT,
    LR_TIMEOUT
};

#define query_t uint8_t

class QueryValues {
private:
    LCD1602Shield * const lcd;
    const Range<query_t> range;
    query_t value;
    const uint8_t base;
    const uint8_t len;
    uint8_t startCol{};
    Buttons prevBtn;
    unsigned long lastPressBtnTime;
    unsigned long lastShowTime;
    boolean visible;
private:
    uint8_t getNumCount(query_t val) const;
    void printBuffer(char * buf, uint8_t len);
    void printValue();
    void hideValue();
    char * valToChars(char buf[], uint8_t & len, query_t val);
    static char encodeChar(uint8_t num);
    static uint8_t decodeChar(char ch);
    void incChar(uint8_t col, bool isInc);
public:
    QueryValues(LCD1602Shield & lcd, query_t defVal, const Range<query_t> &range, uint8_t base = 10);
    ~QueryValues();
    query_t getCurrentVal() const;
    LoopResult loop();
//    virtual void test() = 0;
};


#endif //CONTROLLER_QUERYVALUES_H
