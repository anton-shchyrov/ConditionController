//
// Created by Anton on 24.10.2020.
//

#ifndef CONTROLLER_DOORCONTROLLER_H
#define CONTROLLER_DOORCONTROLLER_H

enum DoorState {
    DOOR_CLOSED,
    DOOR_OPENED,
    DOOR_OPENED_LONG
};

class DoorController {
private:
    unsigned long timeOpen = 0;
private:
    bool isOpened();
public:
    DoorState checkState();
    inline void reset() {
        this->timeOpen = 0;
    }
};


#endif //CONTROLLER_DOORCONTROLLER_H
