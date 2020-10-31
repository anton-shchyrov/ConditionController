//
// Created by Anton on 24.10.2020.
//

#include "DoorController.h"

#include <Arduino.h>

#define WAIT_DOOR (1000 * 60)

bool DoorController::isOpened() {
    return false;
}

DoorState DoorController::checkState() {
    if (!this->isOpened()) {
        this->timeOpen = 0;
        return DOOR_CLOSED;
    }
    if (this->timeOpen == 0)
        this->timeOpen == millis();
    else if (millis() - this->timeOpen >= WAIT_DOOR)
        return DOOR_OPENED_LONG;
    return DOOR_OPENED;
}
