#pragma once

struct BMPManager {
    static void init();
    static float getTemperature();
    static float getPressure();
    static float getAltitude();
    static void scanSensors();
    static const char* getTemperatureString();
};