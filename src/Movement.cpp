#include "Movement.h"
#include <math.h>   // fabsf, powf

namespace MOVEMENT {
    // ===== ÉTATS & PARAMS =====
    MoveEnum currentMove = MoveEnum::NONE;
    float goalDistance = 0.0f;  // cm
    int maxSpeed = 0;
    int minSpeed = 0;
    LineOffsetEnum offsetMode = LineOffsetEnum::AUCUN;

    // Snapshots encodeurs au début d’un virage (cm)
    static float baseRight = 0.0f;
    static float baseLeft  = 0.0f;
    // ===== INIT =====
    void init(){
        WHEEL_PID::init();
        WHEEL_PID::setPIDDesiredPulse(0, 0);
    }

    // ===== API bloquante =====
    void moveForward(float distance_cm, int pMinSpeed, int pMaxSpeed) {
        moveForwardNonBlocking(distance_cm, pMinSpeed, pMaxSpeed);
        waitEndMove();
    }

    void turnRight(float angle_deg, int pMinSpeed, int pMaxSpeed) {
        turnRightNonBlocking(angle_deg, pMinSpeed, pMaxSpeed);
        waitEndMove();
    }

    void turnRightUntilLine(bool waitUntilMiddle,
                              int pMinSpeed,
                              int pMaxSpeed){
        turnRightNonBlocking(360, pMinSpeed, pMaxSpeed);
        if(waitUntilMiddle){
            waitUntilLine(true, false);
        }
        else{
            waitUntilLine();
        }
    }

    void turnLeft(float angle_deg, int pMinSpeed, int pMaxSpeed) {
        turnLeftNonBlocking(angle_deg, pMinSpeed, pMaxSpeed);
        waitEndMove();
    }

     void turnLeftUntilLine(bool waitUntilMiddle,
                              int pMinSpeed,
                              int pMaxSpeed){
        turnLeftNonBlocking(360, pMinSpeed, pMaxSpeed);
        if(waitUntilMiddle){
            waitUntilLine(false, true);
        }else{
            waitUntilLine();
        }
    }

    void moveUntilLine(int pMinSpeed, int pMaxSpeed)
    {
        moveForwardNonBlocking(99999, pMinSpeed, pMaxSpeed);
        waitUntilLine();
    }

    // ===== API non-bloquante =====
    void moveForwardNonBlocking(float distance_cm, int pMinSpeed, int pMaxSpeed) {
        WHEEL_PID::resetCoveredDistance();
        currentMove = MoveEnum::FORWARD;
        offsetMode = LineOffsetEnum::AUCUN;
        goalDistance = distance_cm;   // cm
        minSpeed = pMinSpeed;
        maxSpeed = pMaxSpeed;
    }

    void turnRightNonBlocking(float angle_deg, int pMinSpeed, int pMaxSpeed) {
        WHEEL_PID::resetCoveredDistance();
        currentMove = MoveEnum::TURN_RIGHT;
        goalDistance = angleToDistance(angle_deg);        // cm
        minSpeed = pMinSpeed;
        maxSpeed = pMaxSpeed;
        offsetMode = LineOffsetEnum::AUCUN;
    }

    void turnLeftNonBlocking(float angle_deg, int pMinSpeed, int pMaxSpeed) {
        WHEEL_PID::resetCoveredDistance();
        currentMove = MoveEnum::TURN_LEFT;
        goalDistance = angleToDistance(angle_deg);        // cm
        minSpeed = pMinSpeed;
        maxSpeed = pMaxSpeed;
        offsetMode = LineOffsetEnum::AUCUN;
    }

    // ===== Conversions unités (cm / degrés) =====
    float angleToDistance(float angle_deg){
        // RAYON en m => *100 pour cm
        return angle_deg * DEG_TO_RAD * (RAYON * 100.0f);
    }

    float distanceToAngle(float distance_cm){
        return distance_cm / (DEG_TO_RAD * (RAYON * 100.0f));
    }

    // ===== Attente bloquante =====
    void waitEndMove(){
        while(currentMove != MoveEnum::NONE){
            runMovementController();
            delay(1);
        }
    }

    void waitUntilLine(bool waitUntilRightOut, bool waitUntilLeftOut){

    int linePoolCount = 0;

    bool hasSeenLine = false;  // a-t-on déjà vu de la ligne ?
 
    while(currentMove != MoveEnum::NONE){

        runMovementController();

        delay(2);
 
        unsigned char lineState = LINE::ucReadLineSensors();
 
        bool onAnyLine = (lineState != 0);

        bool rightOn   = (lineState & 0x01);

        bool leftOn    = (lineState & 0x04);
 
        bool rightOut = !rightOn;

        bool leftOut  = !leftOn;
 
        // Mode "simple" : on arrête dès qu'on voit de la ligne un certain nb de fois

        if(!waitUntilRightOut && !waitUntilLeftOut){

            if(onAnyLine){

                linePoolCount++;

                if(linePoolCount >= 3){

                    stop();

                }

            }else{

                linePoolCount = 0;

            }

            continue;

        }
 
        // Mode "waitUntilRightOut / LeftOut"

        if(onAnyLine){

            hasSeenLine = true; // on est passé sur la ligne au moins une fois

        }
 
        // On ne teste la sortie que si on a DÉJÀ vu la ligne

        if(hasSeenLine){

            bool conditionMet = false;
 
            if(waitUntilRightOut && !waitUntilLeftOut){

                conditionMet = rightOut;       // capteur droit n'est plus sur la ligne

            }else if(!waitUntilRightOut && waitUntilLeftOut){

                conditionMet = leftOut;        // capteur gauche n'est plus sur la ligne

            }else if(waitUntilRightOut && waitUntilLeftOut){

                conditionMet = rightOut && leftOut;

            }
 
            if(conditionMet){

                // Alignement final si tu veux

                if(waitUntilRightOut && !waitUntilLeftOut){

                    MOVEMENT::turnRight(5);

                }else if(!waitUntilRightOut && waitUntilLeftOut){

                    MOVEMENT::turnLeft(5);

                }
 
                stop();

            }

        }

    }

}



    // ===== Stop =====
    void stop(){
        WHEEL_PID::setPIDDesiredPulse(0, 0);
        WHEEL_PID::reset();
        currentMove = MoveEnum::NONE;
        goalDistance = 0.0f;
        WHEEL_PID::stopMotor();
    }

    static float computeScaledSpeed(float remainingDistance, float totalDistance,
                                float accelDistance,
                                float minS, float maxS) {
        if (remainingDistance <= 0.0f) return 0.0f;

        float coveredDistance = totalDistance - remainingDistance;

        accelDistance = fminf(accelDistance, 0.5f * totalDistance);

        float accelFactor = constrain(coveredDistance / accelDistance, 0.0f, 1.0f);
        float decelFactor = constrain(remainingDistance / accelDistance, 0.0f, 1.0f);

        float speedFactor = 4.0f * accelFactor * decelFactor;
        speedFactor = constrain(speedFactor, 0.0f, 1.0f);

        return minS + (maxS - minS) * speedFactor;
    }

    // ===== Contrôleur principal =====
    void runMovementController(){
        switch(currentMove){
            case MoveEnum::TURN_RIGHT: {
                //  moyenne des 2 roues 
                float dr = fabsf(WHEEL_PID::getRightCoveredDistance() - baseRight);
                float dl = fabsf(WHEEL_PID::getLeftCoveredDistance()  - baseLeft);
                float coveredDistance = 0.5f * (dr + dl);        // cm
                float remainingDistance = goalDistance - coveredDistance;

                if (remainingDistance <= STOP_TOLERANCE) {
                    stop();
                } else {
                    float speed = computeScaledSpeed(
                        remainingDistance, goalDistance, ACCEL_TURN_DISTANCE, minSpeed, maxSpeed
                    );
                    WHEEL_PID::setPIDDesiredPulse(
                        speed,   // gauche +
                        -speed  // droite -
                    );
                }
                break;
            }

            case MoveEnum::TURN_LEFT: {
                float dr = fabsf(WHEEL_PID::getRightCoveredDistance() - baseRight);
                float dl = fabsf(WHEEL_PID::getLeftCoveredDistance()  - baseLeft);
                float coveredDistance = 0.5f * (dr + dl);        // cm
                float remainingDistance = goalDistance - coveredDistance;

                if (remainingDistance <= STOP_TOLERANCE) {
                    stop();
                } else {
                    float speed = computeScaledSpeed(
                        remainingDistance, goalDistance, ACCEL_TURN_DISTANCE, minSpeed, maxSpeed
                    );
                    WHEEL_PID::setPIDDesiredPulse(
                        -speed, // gauche -
                        speed    // droite +
                    );
                }
                break;
            }

            case MoveEnum::FORWARD: {
                float remainingDistance = goalDistance - WHEEL_PID::getCoveredDistance(); // cm
                if (remainingDistance <= STOP_TOLERANCE) {
                    stop();
                } else {
                    float speed = computeScaledSpeed(
                        remainingDistance, goalDistance, ACCEL_FORWARD_DISTANCE, minSpeed, maxSpeed
                    );

                    float leftSpeed  = speed;
                    float rightSpeed = speed;

                    // Offset de ligne (si utilisé ailleurs)
                    switch(offsetMode){
                        case LineOffsetEnum::PETIT_GAUCHE: leftSpeed  *= 0.85f; break;
                        case LineOffsetEnum::GRAND_GAUCHE: leftSpeed  *= 0.55f; break;
                        case LineOffsetEnum::PETIT_DROITE: rightSpeed *= 0.85f; break;
                        case LineOffsetEnum::GRAND_DROITE: rightSpeed *= 0.55f; break;
                        case LineOffsetEnum::AUCUN: default: break;
                    }

                    WHEEL_PID::setPIDDesiredPulse(
                        leftSpeed,
                        rightSpeed
                    );
                }
                break;
            }

            case MoveEnum::NONE:{
                WHEEL_PID::setPIDDesiredPulse(0, 0);
                break;
            }

            default: break;
        }

        WHEEL_PID::runPIDController();
    }

    
    MoveEnum getCurrentMove(){ return currentMove; }
    void setOffset(LineOffsetEnum mode){ 
        offsetMode = mode;
    }
}
