#pragma once

#include "Movement.h"

#define LEFT_SENSOR_PIN A7
#define MIDDLE_SENSOR_PIN A6
#define RIGHT_SENSOR_PIN A5

constexpr int WHITE_THRESHOLD = 980;

enum StationEnum { NONE, WALL_DODGE, LOSANGE, LOST_LINE, QUILLE};

void vSetupLineSensors();
unsigned char ucReadLineSensors();
void vCourseCorrection();
StationEnum ucDecideStation();
