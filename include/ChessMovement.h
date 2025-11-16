#pragma once

#include <Arduino.h>
#include "Movement.h"

namespace CHESS_MOVEMENT{
    int currentCol = 0;
    int currentRow = 0;
    const float MAX_MOVE_DISTANCE = 99999;

    // --------------------------------- 
    // 7   |   |   |   |   |   |   |   |
    // ---------------------------------
    // 6   |   |   |   |   |   |   |   |
    // ---------------------------------
    // 5   |   |   |   |   |   |   |   |
    // ---------------------------------
    // 4   |   |   |   |   |   |   |   |
    // ---------------------------------   ROW
    // 3   |   |   |   |   |   |   |   |
    // ---------------------------------
    // 2   |   |   |   |   |   |   |   |
    // ---------------------------------
    // 1   |   |   |   |   |   |   |   |
    // ---------------------------------
    // 0   |   |   |   |   |   |   |   |
    // --0---1---2---3---4---5---6---7--
    //              COL

    /**
     * @brief Ce dirige au milieu d'une case
     * 
     * @param col entre 0-7
     * @param row entre 0-7
     */
    void moveTo(int col, int row);
    void moveToDropOff();
    void moveToWhite();
    void moveToBlack();

    void waitEndMoveByLineNb(int nbLine);
}