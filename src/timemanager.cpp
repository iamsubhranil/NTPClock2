#include "timemanager.h"
#include "config.h"
#include "wifimanager.h"

// includes configTime for some reason
#include <Arduino.h>

void TimeManager::init() {
	Configuration::onNTPServerChange([](String &server) { sync(); });
	Configuration::onOffsetMinutesChange([](int &offset) { sync(); });
	Configuration::onOffsetHoursChange([](int &offset) { sync(); });
	sync();
}

void TimeManager::sync() {
	WiFiManager::ensureConnected();
	Serial.printf("[" __FILE__ ":%d] Server: %s offset: %d\n", __LINE__,
	              Configuration::getNTPServer().c_str(),
	              Configuration::getOffsetHours() * 3600 +
	                  Configuration::getOffsetMinutes() * 60);
	configTime(Configuration::getOffsetHours() * 3600 +
	               Configuration::getOffsetMinutes() * 60,
	           0, Configuration::getNTPServer().c_str());
}