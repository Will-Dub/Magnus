#pragma once
#include <Arduino.h>
#include <Lcd.h>
#include <Chess.h>

namespace MENU {
    struct MoveInput {
        char fromCol;
        int fromRow;
        char toCol;
        int toRow;
        bool giveUp;
    };

    bool waitForGameStart();
    MoveInput getUserMove(bool isPreviousMoveInvalid);
    void tick();
    void resetMenuIndex();
    void blinkWord(String text, unsigned int startWord1, unsigned int endWord1,
                   unsigned int startWord2, unsigned int endWord2);
    void blinkChar(String text, int optionCount = 2);
}