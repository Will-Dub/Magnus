#pragma once

#include "Movement.h"

#define LEFT_SENSOR_PIN A7
#define MIDDLE_SENSOR_PIN A6
#define RIGHT_SENSOR_PIN A5

namespace LINE{
    constexpr int WHITE_THRESHOLD = 900;

    void vSetupLineSensors();
    unsigned char ucReadLineSensors();
    void vCourseCorrection();

}