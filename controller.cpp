#include <Arduino.h>
//#include <IRremote.h>

#include "LCD1602Shield.h"
#include "QueryValues.h"
#include <LiquidCrystal.h>

LCD1602Shield lcd(8, 9, 4, 5, 6, 7);

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

//inline void operator delete(void* , unsigned int ) throw() { }

void setup() {
    Serial.begin(9600);
    Serial.println("Test");
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
                query = new QueryValues(lcd, 25, 2);
        }
    } else {
        switch (query->loop()) {
            case LR_SELECT:
                Serial.println(String("SELECT ") + query->getCurrentVal());
                delete query;
                query = nullptr;
                break;
            case LR_TIMEOUT:
                Serial.println(String("TIMEOUT ") + query->getCurrentVal());
                delete query;
                query = nullptr;
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
