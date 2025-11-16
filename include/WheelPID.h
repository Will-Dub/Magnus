#pragma once

#include <LibRobus.h>
#include <Arduino.h>

namespace WHEEL_PID{
    constexpr float PULSE_PER_TURN = 3200.0f;
    constexpr float POUCE_TO_CM = 2.54f;
    constexpr float ROUE_DIAMETRE = 3.0f;
    const float kp = 0.02f;
    const float ki = 0.001f;
    const float kd = 0.00005f;
    const float maxIntegral = 300.0f;

    void init();
    void setPIDDesiredPulse(int, int);
    void runPIDController();
    void stopMotor();
    void reset();
    float getCoveredDistance();
    float getRightCoveredDistance();
    float getLeftCoveredDistance();
    void resetCoveredDistance();
    float pulseToDistance(int pulse);
}