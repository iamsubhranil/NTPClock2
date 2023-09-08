#include "animation.h"
#include "BMPManager.h"
#include "clock.h"
#include "config.h"

#include <Arduino.h>

void Clock::init() {
	Animation::addFrame(6, [](char *text, int textSize, int tick){
        struct tm timeinfo;
        bool      success = getLocalTime(&timeinfo);
        if(!success) {
            memcpy(text, "Bad Time!", textSize);
        } else {
            const char *fmt = tick % 2 ? "%I  %M  %S %p" : "%I. %M. %S %p";
            if(Configuration::getClock24()) {
                fmt = tick % 2 ? "%H  %M  %S" : "%H. %M. %S";
            }
            strftime(text, textSize, fmt, &timeinfo);
        }
    });
    Animation::addFrame(3, [](char *text, int textSize, int tick) {
        struct tm timeinfo;
        bool      success = getLocalTime(&timeinfo);
        if(!success) {
            memcpy(text, "Bad Time!", textSize);
        } else {
            const char *fmt = "%a %e, ";
            int written = strftime(text, textSize, fmt, &timeinfo);
            snprintf(text + written, textSize - written, "%s", BMPManager::getTemperatureString());
        }
    });
}