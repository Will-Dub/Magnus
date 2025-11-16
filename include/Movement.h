#pragma once

#include <Arduino.h>
#include <LibRobus.h>
#include "WheelPID.h"
#include "Line.h"

namespace MOVEMENT {
    // RAYON = demi-empattement en mètres (ex.: empattement 19 cm => rayon 9.5 cm => 0.095 m)
    constexpr float RAYON = (0.18f / 2.0f)*1.05;          // m
    constexpr float ACCEL_FORWARD_DISTANCE = 4.0f;   // cm
    constexpr float ACCEL_TURN_DISTANCE    = 2.0f;   // cm
    constexpr float STOP_TOLERANCE         = 1.0f;   

    constexpr int MAX_TURNING_SPEED   = 5500;
    constexpr int MIN_TURNING_SPEED   = 1500;
    constexpr int MAX_STRAIGHT_SPEED  = 7000;
    constexpr int MIN_STRAIGHT_SPEED  = 1000;

    enum MoveEnum { FORWARD, TURN_LEFT, TURN_RIGHT, NONE };

    // Offset pour le suiveur de ligne
    enum LineOffsetEnum {
        AUCUN,
        PETIT_GAUCHE,
        GRAND_GAUCHE,
        PETIT_DROITE,
        GRAND_DROITE
    };

    
    void init();

    void moveForward(float distance_cm,
                     int pMinSpeed = MIN_STRAIGHT_SPEED,
                     int pMaxSpeed = MAX_STRAIGHT_SPEED);
    void moveForwardNonBlocking(float distance_cm,
                                int pMinSpeed = MIN_STRAIGHT_SPEED,
                                int pMaxSpeed = MAX_STRAIGHT_SPEED);

    void turnRight(float angle_deg,
                   int pMinSpeed = MIN_TURNING_SPEED,
                   int pMaxSpeed = MAX_TURNING_SPEED);
    void turnRightNonBlocking(float angle_deg,
                              int pMinSpeed = MIN_TURNING_SPEED,
                              int pMaxSpeed = MAX_TURNING_SPEED);
    void turnRightUntilLine(int pMinSpeed = MIN_TURNING_SPEED,
                              int pMaxSpeed = MAX_TURNING_SPEED);

    void turnLeft(float angle_deg,
                  int pMinSpeed = MIN_TURNING_SPEED,
                  int pMaxSpeed = MAX_TURNING_SPEED);
    void turnLeftNonBlocking(float angle_deg,
                             int pMinSpeed = MIN_TURNING_SPEED,
                             int pMaxSpeed = MAX_TURNING_SPEED);
    void turnLeftUntilLine(int pMinSpeed = MIN_TURNING_SPEED,
                              int pMaxSpeed = MAX_TURNING_SPEED);

    void moveUntilLine(int pMinSpeed = MIN_STRAIGHT_SPEED,
                                int pMaxSpeed = MAX_STRAIGHT_SPEED);

    void waitUntilLine();

    // conversions : distance en cm, angle en degrés
    float distanceToAngle(float distance_cm);
    float angleToDistance(float angle_deg);

    void stop();
    void waitEndMove();
    void runMovementController();
    void setOffset(LineOffsetEnum mode);
    MoveEnum getCurrentMove();
}
