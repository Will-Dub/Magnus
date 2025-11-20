#include "Line.h"
#include <Arduino.h>

namespace LINE{

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

        if (iLeftSensorValue >= WHITE_THRESHOLD) { bLeftSensor = true;}
        if (iMiddleSensorValue >= WHITE_THRESHOLD) { bMiddleSensor = true;}
        if (iRightSensorValue >= WHITE_THRESHOLD) { bRightSensor = true;}

        unsigned char ucSensorState = ((bLeftSensor << 2) | (bMiddleSensor << 1) | bRightSensor);
        return ucSensorState;
    }

    void vCourseCorrection()
    {
        unsigned char ucSensorState = ucReadLineSensors();
        switch (ucSensorState) {
            case 0b000:
                // No line
                break; 
            case 0b001:
                // Right sensor on line
                MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::GRAND_DROITE);
                break;
            case 0b010:
                // Middle sensor on line
                MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::AUCUN);
                break;
            case 0b011:
                // Middle and right on line
                MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::PETIT_DROITE);
                break;
            case 0b100:
                // Left sensor on line
                MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::GRAND_GAUCHE);
                break;
            case 0b101:
                // Left and right on line
                //IDK THATS FUCKED UP G, GO STRAIGHT I GUESS
                MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::AUCUN);
                break;
            case 0b110:
                // Left and middle on line
                MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::PETIT_GAUCHE);
                break;
            case 0b111:
                // All on line
                MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::AUCUN);
                break;
            default:
                Serial.println("Error: 0_0 au secours!");
                break;
        }
    }
}