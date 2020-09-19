//
// Created by Anton on 06.09.2020.
//

#ifndef CONTROLLER_DIGITQUERYVALUES_H
#define CONTROLLER_DIGITQUERYVALUES_H

#include "CustomQueryValues.h"

class DigitQueryValues : public CustomQueryValues {
private:
    void incChar(uint8_t col, bool isInc);
protected:
    void doPressButton(Buttons btn) override;
public:
    DigitQueryValues(query_t defVal, const Range<query_t> &range, uint8_t base = DEF_BASE);
    ~DigitQueryValues() override;
};

#endif //CONTROLLER_DIGITQUERYVALUES_H
