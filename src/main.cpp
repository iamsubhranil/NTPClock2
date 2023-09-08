#include "animation.h"
#include "BMPManager.h"
#include "clock.h"
#include "config.h"
#include "displaymanager.h"
#include "ledmanager.h"
#include "scheduler.h"
#include "server.h"
#include "storage.h"
#include "timemanager.h"
#include "weathermanager.h"
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
	Animation::init();
	Clock::init();
	
	Scheduler::every(30).minute().perform([]() { TimeManager::sync(); });
	Scheduler::every().second().perform([]() {
		Animation::performTick();
	});

	Scheduler::begin();
}

void loop() {}