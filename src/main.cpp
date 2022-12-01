#include "clock.h"
#include "config.h"
#include "displaymanager.h"
#include "scheduler.h"
#include "server.h"
#include "timemanager.h"
#include "wifimanager.h"

Scheduler scheduler;

void setup() {

	Serial.begin(9600);
	DisplayManager::init();
	WiFiManager::init();
	ServerManager::init();
	TimeManager::init();

	scheduler.every(30).minute().perform([]() { TimeManager::sync(); });
	scheduler.every().second().perform([]() {
		struct tm timeinfo;
		bool      success = getLocalTime(&timeinfo);
		if(!success) {
			DisplayManager::print("Bad Time!");
			TimeManager::sync();
		} else {
			// Serial.println("Time retrieved..");
			Clock::show(timeinfo);
		}
	});

	// Display.setTextAlignment(0, PA_CENTER);
	// Display.setTextAlignment(1, PA_CENTER);
}

void loop() {
	scheduler.run();
	delay(200);
}