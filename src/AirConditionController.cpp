//
// Created by Anton on 24.10.2020.
//

#include "AirConditionController.h"
#include "globals.h"

bool AirConditionController::isPowerOn = false;

void AirConditionController::powerOn() {
    if (!isPowerOn) {
        sendCommand(settings.getOnCommand());
        isPowerOn = true;
    }
}

void AirConditionController::powerOff() {
    if (!isPowerOn) {
        sendCommand(settings.getOffCommand());
        isPowerOn = false;
    }
}

void AirConditionController::applyTemperature(temp_t val) {
    if (val < settings.getMinTemp())
        powerOff();
    else if (val > settings.getMaxTemp())
        powerOn();
}
