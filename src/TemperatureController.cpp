//
// Created by Anton on 30.12.2020.
//

#include "TemperatureController.h"

#define DS18S20 0x10
#define DS1822  0x22
#define DS18B20 0x28

TemperatureController::TemperatureController(uint8_t pin) : driver(pin) {}

void TemperatureController::QueryTemperature() {

}

bool TemperatureController::begin() {
    uint8_t addr[8];
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

    if (OneWire::crc8(addr, 7) != addr[7]) {
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

    this->driver.reset();
    this->driver.select(addr);
    Serial.println("Wait temperature");
    return true;
}
/*
void handleOWIO(byte pin, byte resolution) {
    int owpin = pin;

// Device identifier
    byte dsaddr[8];
    OneWire myds(owpin);
    Serial.println(getdstemp(myds, dsaddr, resolution));

} // run OW sequence

float getdstemp(OneWire myds, byte addr[8], byte resolution) {
    byte present = 0;
    int i;
    byte data[12];
    byte type_s;
    float celsius;
    float fahrenheit;

    switch (addr[0]) {
        case 0x10:
//Serial.println(F("  Chip = DS18S20"));  // or old DS1820
            type_s = 1;
            break;
        case 0x28:
//Serial.println(F("  Chip = DS18B20"));
            type_s = 0;
            break;
        case 0x22:
//Serial.println(F("  Chip = DS1822"));
            type_s = 0;
            break;
        default:
            Serial.println(F("Device is not a DS18x20 family device."));
    }

// Get byte for desired resolution
    uint8_t resbyte;
    switch (resolution) {
        case 12:
            resbyte = 0x7F;
            break;
        case 11:
            resbyte = 0x5F;
            break;
        case 10:
            resbyte = 0x3F;
            break;
        default:
            resbyte = 0x1F;
    }
// Set configuration
//    myds.reset();
//    myds.select(addr);
    myds.write(0x4E);         // Write scratchpad
    myds.write(0);            // TL
    myds.write(0);            // TH
    myds.write(resbyte);         // Configuration Register

    myds.write(0x48);         // Copy Scratchpad

//    myds.reset();
//    myds.select(addr);

    long starttime = millis();
    myds.write(0x44);         // start conversion, with parasite power on at the end
    while (!myds.read()) {
// do nothing
    }
    Serial.print("Conversion took: ");
    Serial.print(millis() - starttime);
    Serial.println(" ms");

//delay(1000);     // maybe 750ms is enough, maybe not
// we might do a ds.depower() here, but the reset will take care of it.

    present = myds.reset();
    myds.select(addr);
    myds.write(0xBE);         // Read Scratchpad

//Serial.print("  Data = ");
//Serial.print(present,HEX);
    Serial.println("Raw Scratchpad Data: ");
    for ( i = 0; i < 9; i++) {           // we need 9 bytes
        data[i] = myds.read();
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
//Serial.print(" CRC=");
//Serial.print(OneWire::crc8(data, 8), HEX);
    Serial.println();

// convert the data to actual temperature

    unsigned int raw = (data[1] << 8) | data[0];
    if (type_s) {
        raw = raw << 3; // 9 bit resolution default
        if (data[7] == 0x10) {
// count remain gives full 12 bit resolution
            raw = (raw & 0xFFF0) + 12 - data[6];
        } else {
            byte cfg = (data[4] & 0x60);
            if (cfg == 0x00) raw = raw << 3;  // 9 bit resolution, 93.75 ms
            else if (cfg == 0x20) raw = raw << 2; // 10 bit res, 187.5 ms
            else if (cfg == 0x40) raw = raw << 1; // 11 bit res, 375 ms
// default is 12 bit resolution, 750 ms conversion time
        }
    }
    celsius = (float)raw / 16.0;
    fahrenheit = celsius * 1.8 + 32.0;
    Serial.print("Temp (C): ");
//Serial.println(celsius);
    return celsius;
}
*/