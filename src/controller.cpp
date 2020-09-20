#include <Arduino.h>
//#include <IRremote.h>

#include "CustomQueryValues.h"
#include "globals.h"
#include "QueryData.h"

Buttons prevBtn = BTN_NONE;

QueryData queryData;

//IRrecv recv(13);

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
    if (!queryData.isInitialized()) {
        Buttons curBtn = LCD1602Shield::detectButton();
        if (curBtn != prevBtn) {
            prevBtn = curBtn;
            if (curBtn == BTN_SELECT)
                queryData.init();
        }
    } else {
        switch (queryData.loop()) {
            case LR_SELECT:
                Serial.println(String("SELECT ") + queryData.done());
                queryData.nextMode();
                queryData.init();
                break;
            case LR_TIMEOUT:
                Serial.println(String("TIMEOUT ") + queryData.done());
                queryData.resetMode();
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

