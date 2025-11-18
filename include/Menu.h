#pragma once
#include <Arduino.h>
#include <Lcd.h>
#include <Chess.h>

namespace MENU {
    struct MoveInput {
        int fromColIndex;
        int fromRowIndex;
        int toColIndex;
        int toRowIndex;
        bool giveUp;
    };

    const int BUTTON_PRESS_TIME_MS = 50;
    const int PIN_OK_BUTTON = 44;
    const int PIN_UP_BUTTON = 45;
    const int PIN_DOWN_BUTTON = 46;
    const int PIN_RIGHT_BUTTON = 47;
    const int PIN_LEFT_BUTTON = 48;

    void init();
    void waitForGameStart();
    MoveInput getUserMove(bool isPreviousMoveInvalid, bool isWhitePlaying);
    void waitForLoseAck();
    void tick();
    void reset();
    void blinkWord(String text, unsigned int startWord1, unsigned int endWord1,
                   unsigned int startWord2, unsigned int endWord2);
    void blinkChar(String text, int optionCount = 2);
    void blinkLine(String text);
}