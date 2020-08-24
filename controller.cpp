#include <Arduino.h>
//#include <IRremote.h>

#include "LCD1602Shield.h"
#include "QueryValues.h"
#include "Settings.h"

enum QueryMode {
    QM_MIN_TEMP,
    QM_MAX_TEMP,
    QM_UNUSED
};

LCD1602Shield lcd(8, 9, 4, 5, 6, 7);
Settings settings;

const String BUTTON_NAMES[] {
        "NONE",
        "UP",
        "DOWN",
        "LEFT",
        "RIGHT",
        "SELECT"
};

Buttons prevBtn = BTN_NONE;

//IRrecv recv(13);

QueryValues * query = nullptr;
QueryMode queryMode = static_cast<QueryMode>(0);

uint8_t getQueryValue() {
    uint8_t res = query->getCurrentVal();
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
    UInt8Range range;
    uint8_t queryVal;
    switch (queryMode) {
        case QM_MIN_TEMP:
            settings.getMinTempRange(range);
            queryVal = settings.getMinTemp();
            break;
        case QM_MAX_TEMP:
            settings.getMaxTempRange(range);
            queryVal = settings.getMaxTemp();
            break;
    }
    query = new QueryValues(lcd, queryVal, range);
}

void setup() {
    Serial.begin(9600);
    Serial.println("Test");
    settings.load();
    byte heart[8] = {
            0b00000,
            0b01010,
            0b11111,
            0b11111,
            0b11111,
            0b01110,
            0b00100,
            0b00000
    };
//    lcd.createChar(0, heart);

//    recv.enableIRIn();
    lcd.begin(16, 2);
//    lcd.setCursor(1, 2);
    lcd.print("01234567", "012");
    uint8_t col, row;
    lcd.getCursor(col, row);
    lcd.print(String(" C:") + col + " R:" + row + " ");
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
/*  switch (button) {
    case BTN_UP:
      printDisplay("UP");
      break;
    case BTN_DOWN:
      printDisplay("DOWN");
      break;
    case BTN_LEFT:
      printDisplay("LEFT");
      break;
    case BTN_RIGHT:
      printDisplay("RIGHT");
      break;
    case BTN_SELECT:
      printDisplay("SELECT");
      break;
    default:
      //printDisplay("Press any key");
      break;
  }
*/}

