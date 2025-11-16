#include "WheelPID.h"

namespace WHEEL_PID{
    unsigned long previousTime = 0;
    int leftDesiredPulse = 0;
    int rightDesiredPulse = 0;
    long leftTotalPulse = 0;
    long rightTotalPulse = 0;
    float lastLeftError = 0;
    float lastRightError = 0;
    float iLeftError = 0;
    float iRightError = 0;
    
    static constexpr float LEFT_SCALE  = 1.00f; // augmente la vitesse de la roue gauche
    static constexpr float RIGHT_SCALE = 1.00f; // ne change pas la droite

    inline float constrainFloat(float value, float min_val, float max_val) {
        if (value < min_val) return min_val;
        if (value > max_val) return max_val;
        return value;
    }

    void init(){
        previousTime = millis();
    }

    void setPIDDesiredPulse(int newLeftDesiredPulse, int newRightDesiredPulse){
        // équilibrage G/D 
        leftDesiredPulse  = (LEFT_SCALE  * newLeftDesiredPulse);
        rightDesiredPulse = (RIGHT_SCALE * newRightDesiredPulse);
    }

    void stopMotor(){
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
        previousTime = millis();
        iLeftError = 0;
        iRightError = 0;
        lastLeftError = 0;
        lastRightError = 0;
    }

    void runPIDController() {
        unsigned long currentTime = millis();
        float elapsedTimeSec = (currentTime - previousTime) / 1000.0f;

        if (leftDesiredPulse == 0 && rightDesiredPulse == 0) {
            stopMotor();
            return;
        }
        if (elapsedTimeSec <= 0.0f) return;
        
        float leftMotorCalculatedPulse = (abs(leftDesiredPulse) * elapsedTimeSec);
        float rightMotorCalculatedPulse = (abs(rightDesiredPulse) * elapsedTimeSec);
        int rawLeftMotorPulse = ENCODER_ReadReset(LEFT);
        int rawRightMotorPulse = ENCODER_ReadReset(RIGHT);
        int rightMotorPulse = abs(rawRightMotorPulse);
        int leftMotorPulse = abs(rawLeftMotorPulse);
        
        leftTotalPulse += rawLeftMotorPulse;
        rightTotalPulse += rawRightMotorPulse;

        // P
        float leftMotorError = leftMotorCalculatedPulse - leftMotorPulse;
        float rightMotorError = rightMotorCalculatedPulse - rightMotorPulse;
        
        // I
        iLeftError += leftMotorError * elapsedTimeSec;
        iRightError += rightMotorError * elapsedTimeSec;
        
        iLeftError = constrainFloat(iLeftError, -maxIntegral, maxIntegral);
        iRightError = constrainFloat(iRightError, -maxIntegral, maxIntegral);
        
        // D
        float dLeftMotorError = (leftMotorError - lastLeftError) / elapsedTimeSec;
        float dRightMotorError = (rightMotorError - lastRightError) / elapsedTimeSec;
        
        // Calculation
        float outLeftMotor = kp * leftMotorError + ki * iLeftError + kd * dLeftMotorError;
        float outRightMotor = kp * rightMotorError + ki * iRightError + kd * dRightMotorError;
        
        // Range 0-1
        outLeftMotor = constrainFloat(outLeftMotor, 0, 1.0);
        outRightMotor = constrainFloat(outRightMotor, 0, 1.0);

        if(leftDesiredPulse < 0){
            outLeftMotor = -outLeftMotor;
        }
        if(rightDesiredPulse < 0){
            outRightMotor = -outRightMotor;
        }

        MOTOR_SetSpeed(RIGHT, outRightMotor);
        MOTOR_SetSpeed(LEFT, outLeftMotor);
        
        lastLeftError = leftMotorError;
        lastRightError = rightMotorError;
        previousTime = currentTime;
        
        /*
        Serial.print(">");
        Serial.print("rawRight:");
        Serial.print(rawRightMotorPulse);
        Serial.print(",");
        Serial.print("rawLeft:");
        Serial.print(rawLeftMotorPulse);
        Serial.print(",");
        Serial.print("P:");
        Serial.print(rightMotorError);
        Serial.print(",");
        Serial.print("I:");
        Serial.print(iLeftError);
        Serial.print(",");
        Serial.print("D:");
        Serial.print(dRightMotorError);
        Serial.print(",");
        Serial.print("rightTotalPulse:");
        Serial.print(rightTotalPulse);
        Serial.print(",");
        Serial.print("OutRight:");
        Serial.print(outRightMotor);
        Serial.print(",");
        Serial.print("DesiredRight:");
        Serial.print(rightDesiredPulse);
        Serial.print(",");
        Serial.print("getRightCoveredDistance:");
        Serial.print(getRightCoveredDistance());
        Serial.println();*/
    }

    void reset(){
        lastLeftError = 0;
        lastRightError = 0;
        iLeftError = 0;
        iRightError = 0;
    }

    // Retourne la distance en cm
    float getCoveredDistance(){
        float combinedAveragePulse = (abs(leftTotalPulse) + abs(rightTotalPulse)) / 2;
        return pulseToDistance(combinedAveragePulse);
    }

    float getRightCoveredDistance() {
        return pulseToDistance(rightTotalPulse);
    }

    float getLeftCoveredDistance(){
        return pulseToDistance(leftTotalPulse);
    }

    float pulseToDistance(int pulse) {
        return abs(pulse) / PULSE_PER_TURN * (PI * ROUE_DIAMETRE) * POUCE_TO_CM;
    }

    void resetCoveredDistance(){
        previousTime = millis();
        leftTotalPulse = 0;
        rightTotalPulse = 0;
    }
}