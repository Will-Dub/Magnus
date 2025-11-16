#include "Movement.h"

namespace MOVEMENT {
    MoveEnum currentMove = MoveEnum::NONE;
    float goalDistance = 0;
    int maxSpeed = 0;
    int minSpeed = 0;
    LineOffsetEnum offsetMode = LineOffsetEnum::AUCUN;

    void init(){
        WHEEL_PID::init();
        WHEEL_PID::setPIDDesiredPulse(0, 0);
    }

    void moveForward(float pDistance, int pMinSpeed, int pMaxSpeed) {
        moveForwardNonBlocking(pDistance, pMinSpeed, pMaxSpeed);
        waitEndMove();
    }

    void moveForwardNonBlocking(float pDistance, int pMinSpeed, int pMaxSpeed) {
        WHEEL_PID::resetCoveredDistance();
        currentMove = MoveEnum::FORWARD;
        offsetMode = LineOffsetEnum::AUCUN;
        goalDistance = pDistance;
        minSpeed = pMinSpeed;
        maxSpeed = pMaxSpeed;
    }

    void turnRight(float pAngle, int pMinSpeed, int pMaxSpeed) {
        turnRightNonBlocking(pAngle, pMinSpeed, pMaxSpeed);
        waitEndMove();
    }

    void turnRightNonBlocking(float pAngle, int pMinSpeed, int pMaxSpeed) {
        WHEEL_PID::resetCoveredDistance();
        currentMove = MoveEnum::TURN_RIGHT;
        goalDistance = angleToDistance(pAngle);
        minSpeed = pMinSpeed;
        maxSpeed = pMaxSpeed;
    }

    void turnLeft(float pAngle, int pMinSpeed, int pMaxSpeed) {
        turnLeftNonBlocking(pAngle, pMinSpeed, pMaxSpeed);
        waitEndMove();
    }

    void turnLeftNonBlocking(float pAngle, int pMinSpeed, int pMaxSpeed) {
        WHEEL_PID::resetCoveredDistance();
        currentMove = MoveEnum::TURN_LEFT;
        goalDistance = angleToDistance(pAngle);
        minSpeed = pMinSpeed;
        maxSpeed = pMaxSpeed;
    }

    float angleToDistance(float angle){
        return angle*DEG_TO_RAD*RAYON;
    }

    float distanceToAngle(float distance){
        return distance/(DEG_TO_RAD*RAYON);
    }

    void waitEndMove(){
        while(currentMove != MoveEnum::NONE){
            runMovementController();
            delay(1);
        }
    }

    void stop(){
        WHEEL_PID::setPIDDesiredPulse(0, 0);
        WHEEL_PID::reset();
        currentMove = MoveEnum::NONE;
        WHEEL_PID::stopMotor();
    }

    float computeScaledSpeed(float remainingDistance, float totalDistance,
                         float accelDistance,
                         float minSpeed, float maxSpeed) {
        if (remainingDistance <= 0) return 0;

        float coveredDistance = totalDistance - remainingDistance;

        float accelFactor = constrain(coveredDistance / accelDistance, 0.0f, 1.0f);
        float decelFactor = constrain(remainingDistance / accelDistance, 0.0f, 1.0f);

        float accelSmooth = pow(accelFactor, 2.0f);
        float decelSmooth = pow(decelFactor, 2.0f);

        float speedFactor = min(accelSmooth, decelSmooth);

        return minSpeed + (maxSpeed - minSpeed) * speedFactor;
    }

    void runMovementController(){
        switch(currentMove){
            case MoveEnum::TURN_RIGHT: {
                float coveredDistance = WHEEL_PID::getRightCoveredDistance();
                float remainingDistance = goalDistance - coveredDistance;
                if (remainingDistance <= STOP_TOLERANCE) {
                    stop();
                } else {
                    float speed = computeScaledSpeed(
                        remainingDistance,
                        goalDistance,
                        ACCEL_TURN_DISTANCE,
                        minSpeed,
                        maxSpeed
                    );
                    WHEEL_PID::setPIDDesiredPulse(speed, -speed);
                }
                break;
            }
            case MoveEnum::TURN_LEFT: {
                float coveredDistance = WHEEL_PID::getRightCoveredDistance();
                float remainingDistance = goalDistance - coveredDistance;
                if (remainingDistance <= STOP_TOLERANCE) {
                    stop();
                } else {
                    float speed = computeScaledSpeed(
                        remainingDistance,
                        goalDistance,
                        ACCEL_TURN_DISTANCE,
                        minSpeed,
                        maxSpeed
                    );
                    WHEEL_PID::setPIDDesiredPulse(-speed, speed);
                }
                break;
            }
            case MoveEnum::FORWARD: {
                float remainingDistance = goalDistance - WHEEL_PID::getCoveredDistance();
                if (remainingDistance <= STOP_TOLERANCE) {
                    stop();
                } else {
                    float speed = computeScaledSpeed(
                        remainingDistance,
                        goalDistance,
                        ACCEL_FORWARD_DISTANCE,
                        minSpeed,
                        maxSpeed
                    );

                    float leftSpeed = speed;
                    float rightSpeed = speed;

                    // Ajoute l'offset
                    switch(offsetMode){
                        case LineOffsetEnum::PETIT_GAUCHE: leftSpeed  *= 0.95f; break;
                        case LineOffsetEnum::GRAND_GAUCHE: leftSpeed  *= 0.85f; break;
                        case LineOffsetEnum::PETIT_DROITE: rightSpeed *= 0.95f; break;
                        case LineOffsetEnum::GRAND_DROITE: rightSpeed *= 0.85f; break;
                        case LineOffsetEnum::AUCUN: break;
                        default: break;
                    }

                    WHEEL_PID::setPIDDesiredPulse(leftSpeed, rightSpeed);
                }
                break;
            }
            case MoveEnum::NONE:{
                WHEEL_PID::setPIDDesiredPulse(0, 0);
                break;
            }
            default: {
                break;
            }
        }

        WHEEL_PID::runPIDController();
    }

    MoveEnum getCurrentMove(){
        return currentMove;
    }

    void setOffset(LineOffsetEnum mode){
        offsetMode = mode;
    }
}