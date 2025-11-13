#pragma once

#include <LiquidCrystal.h>
#include <Arduino.h>

namespace LCD{
    const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
    const int BLINK_TIME_MS = 500;

    void initLcd();
    void print(String text, int line = 0);
    void clear();
    void blinkChessBoard(int col, int row);
}