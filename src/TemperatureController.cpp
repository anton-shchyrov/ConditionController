//
// Created by Anton on 30.12.2020.
//

#include "TemperatureController.h"

#define DS18S20 0x10
#define DS1822  0x22
#define DS18B20 0x28

#define CMD_CONVERT_T        0x44
#define CMD_SCRATCHPAD_COPY  0x48
#define CMD_SCRATCHPAD_WRITE 0x4E

void TemperatureController::sendCommand(const byte * addr, byte command) {
    driver.reset();
    driver.select(addr);
    driver.write(command);
}

void TemperatureController::setResolution(const byte * addr, byte resolution) {
    // Get byte for desired resolution
    uint8_t resbyte;
    switch (resolution) {
        case 12: resbyte = 0x7F; break;
        case 11: resbyte = 0x5F; break;
        case 10: resbyte = 0x3F; break;
        default:
            resbyte = 0x1F;
    }
    // Set configuration
    sendCommand(addr, 0x4E);  // Write scratchpad
    driver.write(0);            // TL
    driver.write(0);            // TH
    driver.write(resbyte);         // Configuration Register

    driver.write(0x48);         // Copy Scratchpad
}

void TemperatureController::convertCommand(const byte * addr) {
    sendCommand(addr, 0x44);
//    driver.reset();
//    driver.select(addr);
//    driver.write(0x44,1);         // start conversion, with parasite power on at the end
}

void TemperatureController::begin(uint8_t pin) {
    this->driver.begin(pin);
}

bool TemperatureController::search(byte * addr) {
    if (!this->driver.search(addr)) {
        Serial.println("Temperature sensor not found");
        return false;
    }
    Serial.print("Addr: ");
    for (int8_t i = 0; i < 8; i++) {
        if (addr[i] < 16)
            Serial.print('0');
        Serial.print(addr[i], HEX);
    }
    Serial.println();

    if (!checkCrc(addr, 7)) {
        Serial.println("CRC is not valid!");
        return false;
    }

    switch (addr[0]) {
        case DS18S20: Serial.println("DS18S20"); break;
        case DS1822 : Serial.println("DS1822"); break;
        case DS18B20: Serial.println("DS18B20"); break;
        default:
            Serial.println("Unknown device");
            return false;
    }

    return true;
}

float TemperatureController::readTemp(const byte * addr) {
    byte data[9];
    byte type_s;

    switch (addr[0]) {
        case DS18S20: type_s = 1; break;
        case DS1822:
        case DS18B20: type_s = 0; break;
        default:
            Serial.println("Device is not a DS18x20 family device.");
    }

    sendCommand(addr, 0xBE);         // Read Scratchpad

    // Raw Scratchpad Data
    for (int i = 0; i < 9; i++) {           // we need 9 bytes
        data[i] = driver.read();
    }
    if (!checkCrc(data, 8))
        return -1;

    // convert the data to actual temperature
    unsigned int raw = (data[1] << 8u) | data[0];
    if (type_s) {
        raw = raw << 3u; // 9 bit resolution default
        if (data[7] == 0x10) {
            // count remain gives full 12 bit resolution
            raw = (raw & 0xFFF0u) + 12 - data[6];
        } else {
            byte cfg = (data[4] & 0x60u);
            switch (cfg) {
                case 0x00: raw = raw << 3u; break;  // 9 bit resolution, 93.75 ms
                case 0x20: raw = raw << 2u; break;  // 10 bit res, 187.5 ms
                case 0x40: raw = raw << 1u; break;  // 11 bit res, 375 ms
                // default is 12 bit resolution, 750 ms conversion time
            }
        }
    }
    float celsius = static_cast<float>(raw) / 16.0f;
    Serial.print("Temp (C): ");
    Serial.println(celsius);
    return celsius;
}

void SingleTemperatureController::begin(uint8_t pin) {
    TemperatureController::begin(pin);
    initialized = search(addr);
}