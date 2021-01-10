#include <Arduino.h>
#include <IRremote.h>

#include "CustomQueryValues.h"
#include "globals.h"
#include "QueryData.h"
#include "DoorController.h"
#include "AirConditionController.h"
#include "TemperatureController.h"

Buttons prevBtn = BTN_NONE;

QueryData queryData;
temp_t prevTemp = 0;

#define IR_PIN 14
#define TEMP_PIN 15

IRrecv recv(IR_PIN);  // pins: SGV
DoorController door;
TemperatureController tc(TEMP_PIN);

temp_t getCurrentTemp() {
    return 23;
}

void setup() {
    Serial.begin(9600);
    Serial.println("Test");
    settings.load();
/*    byte heart[8] = {
            0b00100,
            0b01010,
            0b00100,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000
    };
    lcd.createChar(CHAR_DEG, heart);
*/
    recv.enableIRIn();
    lcd.begin(16, 2);
    tc.Init();
//    lcd.print("Ready");

//    pinMode(IR_PIN, INPUT);
}

void printRemote(const decode_results & res) {
    const char * protocols[MAGIQUEST + 2] = {
            "UNKNOWN",
            "UNUSED",
            "RC5",
            "RC6",
            "NEC",
            "SONY",
            "PANASONIC",
            "JVC",
            "SAMSUNG",
            "WHYNTER",
            "AIWA_RC_T501",
            "LG",
            "SANYO",
            "MITSUBISHI",
            "DISH",
            "SHARP",
            "SHARP_ALT",
            "DENON",
            "LEGO_PF",
            "BOSEWAVE",
            "MAGIQUEST",
    };
    Serial.println("===========");
    Serial.print("decode_type: ");
    Serial.print(protocols[res.decode_type + 1]);
    Serial.print(" (");
    Serial.print(res.decode_type);
    Serial.println(')');
    Serial.print("address: ");
    Serial.println(res.address, 16);
    Serial.print("value: ");
    Serial.print(res.value, 16);
    Serial.print(' ');
    Serial.println(res.value, 2);
    Serial.print("magnitude: ");
    Serial.println(res.magnitude, 16);
    Serial.print("bits: ");
    Serial.println(res.bits);
    //volatile unsigned int *rawbuf;      ///< Raw intervals in 50uS ticks
    Serial.print("rawlen: ");
    Serial.println(res.rawlen);
    Serial.print("overflow: ");
    Serial.println(res.overflow);
}

void mainLoop() {
    temp_t curTemp = getCurrentTemp();
    if (curTemp != prevTemp) {
        prevTemp = curTemp;
        lcd.clear();
        lcd.printLine(String("Temperature:") + curTemp + DEG_STR, 0);
    }
    switch (door.checkState()) {
        case DOOR_CLOSED:
            AirConditionController::applyTemperature(getCurrentTemp());
            break;
        case DOOR_OPENED_LONG:
            AirConditionController::powerOff();
            break;
    }
}

void loop() {
    /**/
    if (!queryData.isInitialized()) {
        Buttons curBtn = lcd.detectButton();
        if (curBtn == BTN_NONE)
            mainLoop();
        if (curBtn != prevBtn) {
            prevBtn = curBtn;
            if (curBtn == BTN_SELECT) {
                queryData.init();
                prevTemp = 0;
            }
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
    /**/
    /** /
    decode_results ir_res;
    memset(&ir_res, 0, sizeof(ir_res));
    if (recv.decode(&ir_res)) {
        recv.dumpPronto(Serial, &ir_res);
        printRemote(ir_res);
        lcd.printLine("0x", 0);
        unsigned long curVal = ir_res.value;
        char chars[8];
        for (int8_t i = 7; i >= 0; i--) {
            int8_t num = curVal % 16;
            if (num <= 9)
                num += '0';
            else
                num += 'A' - 10;
            chars[i] = num;
            curVal /= 16;
        }
        lcd.print(chars, 8);
        delay(50);
        recv.resume();
    }
    /**/
}

