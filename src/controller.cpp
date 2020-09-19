#include <Arduino.h>
//#include <IRremote.h>

#include "CustomQueryValues.h"
#include "DigitQueryValues.h"
#include "NumberQueryValues.h"
#include "globals.h"

enum QueryMode {
    QM_MIN_TEMP,
    QM_MAX_TEMP,
    QM_UNUSED
};

//LCD1602Shield lcd(8, 9, 4, 5, 6, 7);
//Settings settings;

Buttons prevBtn = BTN_NONE;

//IRrecv recv(13);

CustomQueryValues * query = nullptr;
QueryMode queryMode = static_cast<QueryMode>(0);

query_t getQueryValue() {
    query_t res = query->getCurrentVal();
    delete query;
    query = nullptr;
    switch (queryMode) {
        case QM_MIN_TEMP:
            settings.setMinTemp(res);
            break;
        case QM_MAX_TEMP:
            settings.setMaxTemp(res);
            break;
    }
    return res;
}

void createQuery() {
    Range<query_t> range{};
    query_t queryVal;
    switch (queryMode) {
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
        query = new NumberQueryValues(queryVal, range);
    else
        query = new DigitQueryValues(queryVal, range);
}

void setup() {
    Serial.begin(9600);
    Serial.println("Test");
    settings.load();
/*    byte heart[8] = {
            0b00000,
            0b01010,
            0b11111,
            0b11111,
            0b11111,
            0b01110,
            0b00100,
            0b00000
    };
    lcd.createChar(0, heart);
*/
//    recv.enableIRIn();
    lcd.begin(16, 2);
    lcd.print("Ready");
}

void loop() {
    if (!query) {
        Buttons curBtn = LCD1602Shield::detectButton();
        if (curBtn != prevBtn) {
            prevBtn = curBtn;
            if (curBtn == BTN_SELECT)
                createQuery();
        }
    } else {
        switch (query->loop()) {
            case LR_SELECT:
                Serial.println(String("SELECT ") + getQueryValue());
                queryMode = static_cast<QueryMode>(static_cast<int>(queryMode) + 1);
                if (queryMode == QM_UNUSED)
                    queryMode = static_cast<QueryMode>(0);
                createQuery();
                break;
            case LR_TIMEOUT:
                Serial.println(String("TIMEOUT ") + getQueryValue());
                queryMode = static_cast<QueryMode>(0);
                break;
        }
    }
//        decode_results ir_res;
/*        if (recv.decode(&ir_res)) {
            clearLine(0);
            lcd.setCursor(0, 0);
            lcd.print("0x");
            lcd.print(ir_res.value, HEX);
            delay(50);
            recv.resume();
        }
    }
*/}

