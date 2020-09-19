//
// Created by Anton on 22.08.2020.
//

#ifndef CONTROLLER_CUSTOMQUERYVALUES_H
#define CONTROLLER_CUSTOMQUERYVALUES_H

#include "types.h"
#include "LCD1602Shield.h"

enum LoopResult {
    LR_PROCESS,
    LR_SELECT,
    LR_TIMEOUT
};

#define DEF_BASE 10

//#define query_t uint8_t
typedef uint8_t query_t;

class CustomQueryValues {
private:
    Buttons prevBtn;
    unsigned long lastPressBtnTime;
protected:
    const Range<query_t> range;
    query_t value;
    const uint8_t base;
    const uint8_t len;
    const uint8_t startCol;
private:
    uint8_t getNumCount(query_t val) const;
    uint8_t getCurCol() const;
    void printBuffer(char * buf, uint8_t len) const;
    char * valToChars(char buf[], uint8_t & len, query_t val);
    static char encodeChar(uint8_t num);
    static uint8_t decodeChar(char ch);
protected:
    void printValue();
    void hideValue();
    virtual void doPressButton(Buttons btn) = 0;
    virtual void doBeforeLoop();
public:
    CustomQueryValues(query_t defVal, const Range<query_t> &range, uint8_t base = DEF_BASE);
    virtual ~CustomQueryValues();
    query_t getCurrentVal() const;
    LoopResult loop();
};


#endif //CONTROLLER_CUSTOMQUERYVALUES_H
