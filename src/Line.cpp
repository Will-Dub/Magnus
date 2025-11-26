#include "Line.h"
#include <Arduino.h>

namespace LINE{
    static float lineError      = 0.0f;
    static float lastLineError  = 0.0f;
    static float lineIntegral   = 0.0f;
    static unsigned long lastTimeLine = 0;
 
    static constexpr float kpLine = 0.8f;
    static constexpr float kiLine = 0.0f;
    static constexpr float kdLine = 0.4f;

    void vSetupLineSensors()
    {
        pinMode(LEFT_SENSOR_PIN, INPUT);
        pinMode(MIDDLE_SENSOR_PIN, INPUT);
        pinMode(RIGHT_SENSOR_PIN, INPUT);
    }

    unsigned char ucReadLineSensors()
    {
        bool bLeftSensor = false;
        bool bMiddleSensor = false; 
        bool bRightSensor = false;

        int iLeftSensorValue = analogRead(LEFT_SENSOR_PIN);
        int iMiddleSensorValue = analogRead(MIDDLE_SENSOR_PIN);
        int iRightSensorValue = analogRead(RIGHT_SENSOR_PIN);

        Serial.print(iLeftSensorValue);
        Serial.print("-");
        Serial.print(iMiddleSensorValue);
        Serial.print("-");
        Serial.println(iRightSensorValue);

        if (iLeftSensorValue >= WHITE_THRESHOLD) { bLeftSensor = true;}
        if (iMiddleSensorValue >= WHITE_THRESHOLD) { bMiddleSensor = true;}
        if (iRightSensorValue >= WHITE_THRESHOLD) { bRightSensor = true;}

        unsigned char ucSensorState = ((bLeftSensor << 2) | (bMiddleSensor << 1) | bRightSensor);
        return ucSensorState;
    }

    void vCourseCorrection()
    {
        unsigned char ucSensorState = ucReadLineSensors();
    
        // Convertie
        int e = 0;
        switch (ucSensorState) {
            case 0b100: // gauche
                e = +2;
                break;
            case 0b110: // gauche + milieu
                e = +1;
                break;
            case 0b010: // milieu
                e = 0;
                break;
            case 0b011: // milieu + droite
                e = -1;
                break;
            case 0b001: // droite
                e = -2;
                break;
            // weird shit
            case 0b000:
            case 0b101:
            case 0b111:
            default:
                e = 0;
                break;
        }
    
        // PID
        unsigned long now = millis();
        float dt = (now - lastTimeLine) / 1000.0f;
        if (dt <= 0.0f) dt = 0.001f;
        lastTimeLine = now;
    
        lineError = (float)e;
        lineIntegral += lineError * dt;
        if (lineIntegral > 50.0f) lineIntegral = 50.0f;
        if (lineIntegral < -50.0f) lineIntegral = -50.0f;
    
        float dError = (lineError - lastLineError) / dt;
        lastLineError = lineError;
    
        float correction = kpLine * lineError
                        + kiLine * lineIntegral
                        + kdLine * dError;
    
        // Traduit en offset
        if (correction > 1.5f) {
            MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::GRAND_GAUCHE);
        } else if (correction > 0.5f) {
            MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::PETIT_GAUCHE);
        } else if (correction < -1.5f) {
            MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::GRAND_DROITE);
        } else if (correction < -0.5f) {
            MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::PETIT_DROITE);
        } else {
            MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::AUCUN);
        }
    }
}