#pragma once

#include <Arduino.h>
#include <LibRobus.h>
#include "WheelPID.h"

namespace MOVEMENT {
    constexpr float RAYON (0.19 / 2.0);
    constexpr float ACCEL_FORWARD_DISTANCE = 9.0f;
    constexpr float ACCEL_TURN_DISTANCE = 4.8f;
    constexpr float STOP_TOLERANCE = 0.1f;
    constexpr int MAX_TURNING_SPEED = 4500;
    constexpr int MIN_TURNING_SPEED = 1500;
    constexpr int MAX_STRAIGHT_SPEED = 6000;
    constexpr int MIN_STRAIGHT_SPEED = 1000;

    enum MoveEnum { FORWARD, TURN_LEFT, TURN_RIGHT, NONE };

    // Offset pour le suiveur de ligne
    enum LineOffsetEnum {
        AUCUN,
        PETIT_GAUCHE,
        GRAND_GAUCHE,
        PETIT_DROITE,
        GRAND_DROITE
    };

    //function prototypes
    void init();

    void moveForward(float pDistance, int pMinSpeed = MIN_STRAIGHT_SPEED, int pMaxSpeed = MAX_STRAIGHT_SPEED);
    void moveForwardNonBlocking(float pDistance, int pMinSpeed = MIN_STRAIGHT_SPEED, int pMaxSpeed = MAX_STRAIGHT_SPEED);

    void turnRight(float pAngle, int pMinSpeed = MIN_TURNING_SPEED, int pMaxSpeed = MAX_TURNING_SPEED);
    void turnRightNonBlocking(float pAngle, int pMinSpeed = MIN_TURNING_SPEED, int pMaxSpeed = MAX_TURNING_SPEED);

    void turnLeft(float pAngle, int pMinSpeed = MIN_TURNING_SPEED, int pMaxSpeed = MAX_TURNING_SPEED);
    void turnLeftNonBlocking(float pAngle, int pMinSpeed = MIN_TURNING_SPEED, int pMaxSpeed = MAX_TURNING_SPEED);

    float distanceToAngle(float angle);
    float angleToDistance(float distance);
    void stop();
    void waitEndMove();
    void runMovementController();
    void setOffset(LineOffsetEnum mode);
    MoveEnum getCurrentMove();
}