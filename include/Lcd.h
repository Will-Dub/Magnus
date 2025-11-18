#pragma once

#include <LiquidCrystal.h>
#include <Arduino.h>

namespace LCD{
    const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
    const int BLINK_TIME_MS = 500;
    const int mapXPixel1Table[8] = {0, 1, 1, 1, 2, 2, 3, 3};
    const int mapXPixel2Table[8] = {0, 1, 1, 2, 2, 2, 3, 3};
    const int mapPosToIndex[2][4] = {{4, 5, 6, 7}, {0, 1, 2, 3}};
    const int mapYTable[8] = {0, 0, 0, 0, 1, 1, 1, 1};
    const int mapXToPixel1Pos[8] = {1, 4, 2, 0, 3, 1, 4, 2};
    const int mapXToPixel2Pos[8] = {0, 3, 1, 4, 2, 0, 3, 1};
    const int mapYToPixelPos[8] = {3, 2, 1, 0, 3, 2, 1, 0};

    void initLcd();
    void print(String text, int line = 0);
    void clear();
    void blinkChessBoard(int col, int row, bool isInverse);
    void tickBlinkChessBoard();
}