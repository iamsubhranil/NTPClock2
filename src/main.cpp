#include "animation.h"
#include "BMPManager.h"
#include "config.h"
#include "displaymanager.h"
#include "ledmanager.h"
#include "scheduler.h"
#include "server.h"
#include "storage.h"
#include "timemanager.h"
#include "wifimanager.h"

void setup() {

	Serial.begin(9600);
	Scheduler::init();
	Storage::init();
	Configuration::load();
	DisplayManager::init();
	BMPManager::init();
	LEDManager::init();
	WiFiManager::init();
	ServerManager::init();
	TimeManager::init();

	Animation::build();
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

	Scheduler::every(30).minute().perform([]() { TimeManager::sync(); });
	Scheduler::every().second().perform([]() {
		Animation::performTick();
	});

	Scheduler::begin();
}

void loop() {}