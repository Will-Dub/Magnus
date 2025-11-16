#include "Line.h"
#include "ColorSquares.h" 
#include <Arduino.h>

StationEnum ucStation = NONE;

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
            Serial.println("No line detected");
            if (ucStation == StationEnum::NONE) {/* LOST THE LINE, SWEEPING TO FIND IT AGAIN */ }
            break; 
        case 0b001:
            Serial.println("Right sensor on line");
            if (ucStation == StationEnum::NONE)
            {
                Movement::setOffset(Movement::LineOffsetEnum::GRAND_DROITE);
            }
            break;
        case 0b010:
            Serial.println("Middle sensor on line");
            if (ucStation == StationEnum::NONE)
            {
                Movement::setOffset(Movement::LineOffsetEnum::AUCUN);
            }
            break;
        case 0b011:
            Serial.println("Middle and Right sensor on line");
            if (ucStation == StationEnum::NONE)
            {
                Movement::setOffset(Movement::LineOffsetEnum::PETIT_DROITE);
            }
            Movement::setOffset(Movement::LineOffsetEnum::PETIT_DROITE);
            break;
        case 0b100:
            Serial.println("Left sensor on line");
            if (ucStation == StationEnum::NONE)
            {
                Movement::setOffset(Movement::LineOffsetEnum::GRAND_GAUCHE);
            }
            Movement::setOffset(Movement::LineOffsetEnum::GRAND_GAUCHE);
            break;
        case 0b101:
            Serial.println("Left and Right sensor on line");
            //IDK THATS FUCKED UP G, GO STRAIGHT I GUESS
            if (ucStation == StationEnum::NONE)
            {
                Movement::setOffset(Movement::LineOffsetEnum::AUCUN);
            }
            Movement::setOffset(Movement::LineOffsetEnum::AUCUN);
            break;
        case 0b110:
            Serial.println("Left and Middle sensor on line");
            if (ucStation == StationEnum::NONE)
            {
                Movement::setOffset(Movement::LineOffsetEnum::PETIT_GAUCHE);
            }
            break;
        case 0b111:
            Serial.println("All sensors on line");
            if (ucStation == StationEnum::NONE)
            {
                Movement::setOffset(Movement::LineOffsetEnum::AUCUN);
            }
            break;
        default:
            Serial.println("Error: 0_0 au secours!");
            break;
    }
}

StationEnum ucDecideStation()
{
    static bool bPinkStationFlag = false; // TO KNOW IF WE HAVE PASSED THE PINK STATION ALREADY
    static bool bBlueStationFlag = false; // TO KNOW IF WE HAVE PASSED THE BLUE STATION ALREADY
    static bool bGreenStationFlag = false; // TO KNOW IF WE HAVE PASSED THE GREEN STATION ALREADY
    static bool bYellowStationFlag = false; // TO KNOW IF WE HAVE PASSED THE YELLOW STATION ALREADY

    unsigned char ucColor = ucDetectColorSquare();
    StationEnum ucCurrentStation = StationEnum::NONE;

    switch (ucColor) {
        case NO_SQUARE:
            Serial.println("No color square detected");
            ucCurrentStation = StationEnum::NONE;
            break;

        case PINK_SQUARE:
            Serial.println("Pink square detected");
            if (bPinkStationFlag == false)
            {
                ucCurrentStation = StationEnum::QUILLE;
            }
            bPinkStationFlag = true;
            break;

        case GREEN_SQUARE:
            Serial.println("Green square detected");
            if (bGreenStationFlag == false)
            {
                ucCurrentStation = StationEnum::LOST_LINE;
            }
            bGreenStationFlag = true;
            break;

        case BLUE_SQUARE:
            Serial.println("Blue square detected");
            if (bBlueStationFlag == false)
            {
                ucCurrentStation = StationEnum::LOSANGE;
            }
            bBlueStationFlag = true;
            break;

        case YELLOW_SQUARE:
            Serial.println("Yellow square detected");
            if (bYellowStationFlag == false)
            {
                ucCurrentStation = StationEnum::WALL_DODGE;
            }
            bYellowStationFlag = true;
            break;

        case WTF_KINDA_COLOR_SQUARE:
            Serial.println("Unrecognized color square detected");
            ucCurrentStation = StationEnum::NONE;
            break;

        default:
            Serial.println("Error: 0_0 au secours!");
            ucCurrentStation = StationEnum::NONE;
            break;
    }
    return ucCurrentStation;
}