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
    void sendCommand(const byte * addr, byte command);
    static inline bool checkCrc(byte * data, byte len) {
        return OneWire::crc8(data, len) == data[len];
    }
    void setResolution(const byte * addr, byte resolution);
    void convertCommand(const byte * addr);
public:
    virtual void begin(uint8_t pin);
    bool search(byte * addr);
    inline void queryTemperature(const byte * addr, byte resolution) {
        setResolution(addr, resolution);
        convertCommand(addr);
    }
    float readTemp(const byte * addr);
    inline virtual bool ready() {
        return driver.read();
    }
};

class SingleTemperatureController : private TemperatureController {
private:
    byte addr[8] = {};
    bool initialized = false;
public:
    void begin(uint8_t pin) override;
    inline void queryTemperature(byte resolution) {
        if (initialized)
            TemperatureController::queryTemperature(addr, resolution);
    }
    inline float readTemp() {
        return TemperatureController::readTemp(addr);
    }
    inline bool getInitialized() {
        return initialized;
    }
    inline bool ready() override {
        return initialized && TemperatureController::ready();
    }
};

#endif //CONTROLLER_TEMPERATURECONTROLLER_H
