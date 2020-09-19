//
// Created by Anton on 19.09.2020.
//

#ifndef CONTROLLER_QUERYDATA_H
#define CONTROLLER_QUERYDATA_H

#include "CustomQueryValues.h"

enum QueryMode {
    QM_MIN_TEMP,
    QM_MAX_TEMP,
    QM_UNUSED
};

class QueryData {
private:
    CustomQueryValues * query = nullptr;
    QueryMode queryMode = static_cast<QueryMode>(0);
public:
    void init();
    query_t getValue();
    inline bool isInitialized() const {
        return this->query != nullptr;
    }
    inline LoopResult loop() const {
        return (this->isInitialized()) ? this->query->loop() : LR_PROCESS;
    }
    void nextMode();
    inline void resetMode() {
        this->queryMode = static_cast<QueryMode>(0);
    }
};


#endif //CONTROLLER_QUERYDATA_H
