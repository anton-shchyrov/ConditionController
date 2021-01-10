//
// Created by Anton on 30.12.2020.
//

#ifndef CONTROLLER_TEMPERATURECONTROLLER_H
#define CONTROLLER_TEMPERATURECONTROLLER_H

#include <OneWire.h>

class TemperatureController {
private:
    OneWire driver;
private:
    void QueryTemperature();
public:
    TemperatureController(uint8_t pin);
    bool Init();
};


#endif //CONTROLLER_TEMPERATURECONTROLLER_H
