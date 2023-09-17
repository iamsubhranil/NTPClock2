#include "time2.h"
#include "config.h"
#include "scheduler.h"
#include "displaymanager.h"

void Timer::init() {
    Scheduler::every().second().perform([]{
        if(!Configuration::getAutoTurnOff())
            return;
        struct tm timeinfo;
        bool      success = getLocalTime(&timeinfo);
        if(success) {
            Time displayOff = Configuration::getTurnOffTime();
            if(timeinfo.tm_hour == displayOff.hours && timeinfo.tm_min == displayOff.minutes) {
                Configuration::setDisplayState(false);
            }
            Time displayOn = Configuration::getTurnOnTime();
            if(timeinfo.tm_hour == displayOn.hours && timeinfo.tm_min == displayOn.minutes) {
                Configuration::setDisplayState(true);
            }
        }
    });
}