#pragma once

#include <Arduino.h>
#include "Movement.h"

namespace CHESS_MOVEMENT{
    int currentCol = 0;
    int currentRow = 0;

    void moveTo(int col, int row);
    void moveToDropOff();
    void moveToWhite();
    void moveToBlack();
}