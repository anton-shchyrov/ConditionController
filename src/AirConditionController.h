//
// Created by Anton on 24.10.2020.
//

#ifndef CONTROLLER_AIRCONDITIONCONTROLLER_H
#define CONTROLLER_AIRCONDITIONCONTROLLER_H

#include <IRremote.h>
#include "globals.h"

class AirConditionController {
private:
    static IRsend send;
private:
    inline static void sendCommand(unsigned long command) {
        send.sendNEC(command, 32);
    }
public:
    inline static void powerOn() {
        sendCommand(settings.getOnCommand());
    }
    inline static void powerOff() {
        sendCommand(settings.getOffCommand());
    }
};


#endif //CONTROLLER_AIRCONDITIONCONTROLLER_H
