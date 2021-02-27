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

#define IR_PIN 13  // pins: SGV
#define TEMP_PIN 12

DoorController door;
SingleTemperatureController tc;

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
    IrReceiver.begin(IR_PIN, DISABLE_LED_FEEDBACK);
    lcd.begin(16, 2);
//    pinMode(TEMP_PIN, OUTPUT);
//    digitalWrite(TEMP_PIN, LOW);
    tc.begin(TEMP_PIN);
    tc.queryTemperature(12);
    lcd.print("Ready");

//    pinMode(IR_PIN, INPUT);
}

void printRemote(const IRData & res) {
    Serial.println("===========");
    Serial.print("decodedRawData: ");
    Serial.print(getProtocolString(res.protocol));
    Serial.print(" (");
    Serial.print(res.decodedRawData);
    Serial.println(')');
    Serial.print("address: ");
    Serial.println(res.address, 16);
    Serial.print("command: ");
    Serial.print(res.command, 16);
}

void mainLoop() {
    if (tc.ready()) {
        float curTemp = tc.readTemp();
        tc.queryTemperature(12);
        temp_t curTempI = round(curTemp);
        if (curTempI != prevTemp) {
            prevTemp = curTempI;
            lcd.clear();
            lcd.print(
                "Temperature:",
                String("      ") + curTemp + DEG_STR
                );
        }
    }
    switch (door.checkState()) {
        case DOOR_CLOSED:
            AirConditionController::applyTemperature(prevTemp);
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
    if (IrReceiver.decode()) {
        printRemote(IrReceiver.decodedIRData);
        lcd.printLine("0x", 0);
        unsigned long curVal = IrReceiver.decodedIRData.decodedRawData;
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
        IrReceiver.resume();
    }
    /**/
}

