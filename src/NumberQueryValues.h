//
// Created by Anton on 06.09.2020.
//

#ifndef CONTROLLER_NUMBERQUERYVALUES_H
#define CONTROLLER_NUMBERQUERYVALUES_H

#include "CustomQueryValues.h"

class NumberQueryValues : public CustomQueryValues {
private:
    unsigned long lastShowTime;
    boolean visible;
private:
    void incValue(bool isInc);
protected:
    void doBeforeLoop() override;
    void doPressButton(Buttons btn) override;
public:
    NumberQueryValues(LCD1602Shield &lcd, query_t defVal, const Range<query_t> &range, uint8_t base = DEF_BASE);
};


#endif //CONTROLLER_NUMBERQUERYVALUES_H
