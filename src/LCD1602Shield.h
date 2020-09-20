//
// Created by Anton on 16.08.2020.
//

#ifndef CONTROLLER_LCD1602SHIELD_H
#define CONTROLLER_LCD1602SHIELD_H

#include <Arduino.h>
#include <LiquidCrystal.h>

enum Buttons {
    BTN_NONE,
    BTN_UP,
    BTN_DOWN,
    BTN_LEFT,
    BTN_RIGHT,
    BTN_SELECT
};

class LCD1602Shield : private LiquidCrystal {
private:
    uint8_t colPos;
    uint8_t rowPos;
private:
    size_t applyPrint(size_t cnt);
//    using LiquidCrystal::print;
public:
    LCD1602Shield(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
    LCD1602Shield(const LCD1602Shield&) = delete;
    LCD1602Shield& operator=(const LCD1602Shield&) = delete;
    void begin(uint8_t cols, uint8_t rows, uint8_t charsize = 0);
    void clear();
    void getCursor(uint8_t & col, uint8_t & row) const;
    void setCursor(uint8_t col, uint8_t row);
    void showCursor(bool visible);
    void clearLine(uint8_t line);
    void printLine(const String & msg, uint8_t line);
    size_t print(const char *string, int len);
    size_t print(const char *string);
    size_t print(const String & msg);
    void print(const String & msg1, const String & msg2);
    void print(const char * msg1, const char * msg2);
    // buttons
    static Buttons detectButton();
    // inherited
    using LiquidCrystal::begin;
    using LiquidCrystal::write;
};


#endif //CONTROLLER_LCD1602SHIELD_H
