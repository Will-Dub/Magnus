#pragma once

#include <Arduino.h>
#include "Movement.h"

namespace CHESS_MOVEMENT
{
    const float MAX_MOVE_DISTANCE = 99999;
    const float ROBUS_RADIUS_CM = 15/2;

    //                 [BLACK]
    //    --------------------------------- 
    //    7   |   |   |   |   |   |   |   |
    //    ---------------------------------
    //    6   |   |   |   |   |   |   |   |
    //    ---------------------------------
    //    5   |   |   |   |   |   |   |   |
    //    ---------------------------------
    //    4   |   |   |   |   |   |   |   |
    //    ---------------------------------   ROW
    //    3   |   |   |   |   |   |   |   |
    //    ---------------------------------
    //    2   |   |   |   |   |   |   |   |
    //    ---------------------------------
    //    1   |   |   |   |   |   |   |   |
    //    ---------------------------------
    //    0   |   |   |   |   |   |   |   |
    //(D)---0--1---2---3---4---5---6---7--
    //                 [WHITE]
    //                   COL
    // (D)=Dropoff

    // COL et ROW doivent être entre 0-7
    void moveFromWhiteToSquare(int destCol, int destRow);
    void moveFromBlackToSquare(int destCol, int destRow);
    void moveFromSquareToBlack(int currentCol, int currentRow);
    void moveFromSquareToWhite(int currentCol, int currentRow);
    void moveFromDropOffToSquare(int destCol, int destRow);
    // TODO:
    void moveSquareToSquare(int currentCol, int currentRow, int destCol, int destRow);
    void moveFromSquareToDropOff(int currentCol, int currentRow);
    void moveFromDropOffToWhite();

    //void moveToWhite();
    //void moveToBlack();

    void waitEndMoveByLineNb(int nbLine);
    void waitEndMoveByEndLine();
}