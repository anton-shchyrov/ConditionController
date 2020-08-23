//
// Created by Anton on 16.08.2020.
//

#include "LCD1602Shield.h"

#define BLANK_LINE "                "  // 16 chars

LCD1602Shield::LCD1602Shield(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) :
    LiquidCrystal(rs, enable, d0, d1, d2, d3) {}

size_t LCD1602Shield::applyPrint(size_t cnt) {
    colPos += cnt;
    return cnt;
}

void LCD1602Shield::begin(uint8_t cols, uint8_t rows, uint8_t charsize) {
    LiquidCrystal::begin(cols, rows, charsize);
}

void LCD1602Shield::getCursor(uint8_t & col, uint8_t & row) const {
    col = colPos;
    row = rowPos;
}

void LCD1602Shield::setCursor(uint8_t col, uint8_t row) {
    LiquidCrystal::setCursor(col, row);
    colPos = col;
    rowPos = row;
}

void LCD1602Shield::showCursor(bool visible) {
    if (visible)
        blink();
    else
        noBlink();
}

void LCD1602Shield::clearLine(uint8_t line) {
    setCursor(0, line);
    LiquidCrystal::print(BLANK_LINE);
    setCursor(0, line);
}

void LCD1602Shield::printLine(const String & msg, uint8_t line) {
    clearLine(line);
    print(msg);
}

size_t LCD1602Shield::print(const char *string, int len) {
    return applyPrint(LiquidCrystal::write(string, len));
}

size_t LCD1602Shield::print(const String & msg) {
    return applyPrint(LiquidCrystal::print(msg));
}

void LCD1602Shield::print(const String & msg1, const String & msg2) {
    clear();
    print(msg1);
    setCursor(0, 1);
    print(msg2);
}

Buttons LCD1602Shield::detectButton() {
    int keyAnalog =  analogRead(A0);
    if (keyAnalog < 100) {
        // Значение меньше 100 – нажата кнопка right
        return BTN_RIGHT;
    } else if (keyAnalog < 200) {
        // Значение больше 100 (иначе мы бы вошли в предыдущий блок результата сравнения, но меньше 200 – нажата кнопка UP
        return BTN_UP;
    } else if (keyAnalog < 400) {
        // Значение больше 200, но меньше 400 – нажата кнопка DOWN
        return BTN_DOWN;
    } else if (keyAnalog < 600) {
        // Значение больше 400, но меньше 600 – нажата кнопка LEFT
        return BTN_LEFT;
    } else if (keyAnalog < 800) {
        // Значение больше 600, но меньше 800 – нажата кнопка SELECT
        return BTN_SELECT;
    } else {
        // Все остальные значения (до 1023) будут означать, что нажатий не было
        return BTN_NONE;
    }
}

