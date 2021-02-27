//
// Created by Anton on 24.10.2020.
//

#ifndef CONTROLLER_AIRCONDITIONCONTROLLER_H
#define CONTROLLER_AIRCONDITIONCONTROLLER_H

#include <IRremoteInt.h>
#include "Settings.h"

class AirConditionController {
private:
    static bool isPowerOn;
private:
    inline static void sendCommand(unsigned long command) {
        IrSender.sendNEC(command, 32, 1);
    }
public:
    static void powerOn();
    static void powerOff();
    static void applyTemperature(temp_t val);
};


#endif //CONTROLLER_AIRCONDITIONCONTROLLER_H
