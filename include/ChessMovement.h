#pragma once

#include <Arduino.h>
#include "Movement.h"

namespace CHESS_MOVEMENT{
    int currentCol = 0;
    int currentRow = 0;
    const float MAX_MOVE_DISTANCE = 99999;

    void moveTo(int col, int row);
    void moveToDropOff();
    void moveToWhite();
    void moveToBlack();

    void waitEndMoveByLineNb(int nbLine);
}