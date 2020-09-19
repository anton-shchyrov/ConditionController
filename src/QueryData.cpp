//
// Created by Anton on 19.09.2020.
//

#include "QueryData.h"
#include "globals.h"
#include "NumberQueryValues.h"
#include "DigitQueryValues.h"

void QueryData::init() {
    if (this->isInitialized())
        return;
    Range<query_t> range{};
    query_t queryVal;
    switch (this->queryMode) {
        case QM_MIN_TEMP:
            Settings::getMinTempRange(range);
            queryVal = settings.getMinTemp();
            break;
        case QM_MAX_TEMP:
            settings.getMaxTempRange(range);
            queryVal = settings.getMaxTemp();
            break;
    }
    if (range.max - range.min <= 9)
        this->query = new NumberQueryValues(queryVal, range);
    else
        this->query = new DigitQueryValues(queryVal, range);
}

query_t QueryData::getValue() {
    if (!this->isInitialized())
        return -1;
    query_t res = this->query->getCurrentVal();
    delete this->query;
    this->query = nullptr;
    switch (this->queryMode) {
        case QM_MIN_TEMP:
            settings.setMinTemp(res);
            break;
        case QM_MAX_TEMP:
            settings.setMaxTemp(res);
            break;
    }
    return res;
}

void QueryData::nextMode() {
    this->queryMode = static_cast<QueryMode>(static_cast<int>(queryMode) + 1);
    if (this->queryMode == QM_UNUSED)
        this->resetMode();
}
