#pragma once

#include <Arduino.h>
#include "Movement.h"

namespace CHESS_MOVEMENT
{
    const float MAX_MOVE_DISTANCE = 99999;
    const float ROBUS_RADIUS_CM = 23.0f/2;
    const int MIN_DETECTED_LINE_READ_COUNT = 1;
    const int MIN_OFF_LINE_BEFORE_NEXT = 45;
    const int TURN_ANGLE = 70;
    const int FULL_TURN_ANGLE = 155;
    
    //                 [BLACK]
    //    --------------------------------- 
    //    7   |   |   |   |   |   |   |   |
    //    ---------------------------------
    //    6   |   |   |   |   |   |   |   |
    //    ---------------------------------
    //    5   |   |   |   |   |   |   |   |
    //    ---------------------------------
    //    4   |   |   |   |   |   |   |   |
    //    ---------------------------------  ROW
    //    3   |   |   |   |   |   |   |   |
    //    ---------------------------------
    //    2   |   |   |   |   |   |   |   |
    //    ---------------------------------
    //    1   |   |   |   |   |   |   |   |
    //    ---------------------------------
    //    0   |   |   |   |   |   |   |   |
    //    --0---1---2---3---4---5---6---7--
    //                 [WHITE]
    //                   COL
    // (D)=Dropoff

    // COL et ROW doivent être entre 0-7
    void moveSquareToSquare(int currentCol, int currentRow, int destCol, int destRow);
    void moveFromSquareToWhite(int currentCol, int currentRow);
    void moveFromDropOffToSquare(int destCol, int destRow);

    //TESTER
    void moveFromSquareToDropOff(int currentCol, int currentRow);
    void moveFromWhiteToSquare(int destCol, int destRow);
    void moveFromDropOffToWhite();
    void moveFromBlackToWhite();
    void moveFromBlackToSquare(int destCol, int destRow);
    void moveFromSquareToBlack(int currentCol, int currentRow);

    void waitEndMoveByLineNb(int nbLine);
    void waitEndMoveByEndLine();

    inline void turnLeft();
    inline void turnRight();
    inline void fullTurn();
    inline void moveByLineNb(int nbLine);
}