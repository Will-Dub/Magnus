#pragma once

#include <Arduino.h>
#include "Movement.h"

namespace CHESS_MOVEMENT
{
    const float MAX_MOVE_DISTANCE = 99999;
    const float ROBUS_RADIUS_CM = 15;

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
    //(D)-(O)0--1---2---3---4---5---6---7--
    //                 [WHITE]
    //                   COL
    // (O)=Origin (D)=Dropoff

    /**
     * @brief Ce dirige au milieu d'une case(assume que le robot est à l'origine et regarde le nord)
     * 
     * @param destCol entre 0-7
     * @param destRow entre 0-7
     */
    void moveFromWhiteToSquare(int destCol, int destRow);

    /**
     * @brief Bouge d'une case vers la zone de drop(assume que le robot est au milieu de la case et ragarde le nord)
     * 
     * @param currentCol entre 0-7
     * @param currentRow entre 0-7
     */
    void moveFromSquareToDropOff(int currentCol, int currentRow);
    //void moveToWhite();
    //void moveToBlack();

    void waitEndMoveByLineNb(int nbLine);
    void waitEndMoveByEndLine();
}