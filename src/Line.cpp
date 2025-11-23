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
        switch (ucSensorState) {
            case 0b000:
                Serial.println("No line");
                // No line
                break; 
            case 0b001:
                Serial.println("Right sensor on line");
                // Right sensor on line
                MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::GRAND_DROITE);
                break;
            case 0b010:
                Serial.println("Middle sensor on line");
                // Middle sensor on line
                MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::AUCUN);
                break;
            case 0b011:
                Serial.println("Middle and right on line");
                // Middle and right on line
                MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::PETIT_DROITE);
                break;
            case 0b100:
                Serial.println("Left sensor on line");
                // Left sensor on line
                MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::GRAND_GAUCHE);
                break;
            case 0b101:
                Serial.println("Left and right on line");
                // Left and right on line
                //IDK THATS FUCKED UP G, GO STRAIGHT I GUESS
                MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::AUCUN);
                break;
            case 0b110:
                Serial.println("Left and middle on line");
                // Left and middle on line
                MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::PETIT_GAUCHE);
                break;
            case 0b111:
                Serial.println("All on line");
                // All on line
                MOVEMENT::setOffset(MOVEMENT::LineOffsetEnum::AUCUN);
                break;
            default:
                Serial.println("Error: 0_0 au secours!");
                break;
        }
    }
}