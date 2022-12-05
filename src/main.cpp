#include "clock.h"
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
	Storage::init();
	Configuration::load();
	DisplayManager::init();
	LEDManager::init();
	WiFiManager::init();
	ServerManager::init();
	TimeManager::init();

	Scheduler::every(30).minute().perform([]() { TimeManager::sync(); });
	Scheduler::every().second().perform([]() {
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
	Scheduler::run();
	delay(200);
}