//
// Created by Anton on 24.10.2020.
//

#ifndef CONTROLLER_AIRCONDITIONCONTROLLER_H
#define CONTROLLER_AIRCONDITIONCONTROLLER_H

#include <IRremote.h>
#include "Settings.h"

class AirConditionController {
private:
    static IRsend send;
    static bool isPowerOn;
private:
    inline static void sendCommand(unsigned long command) {
        send.sendNEC(command, 32);
    }
public:
    static void powerOn();
    static void powerOff();
    static void applyTemperature(temp_t val);
};


#endif //CONTROLLER_AIRCONDITIONCONTROLLER_H
