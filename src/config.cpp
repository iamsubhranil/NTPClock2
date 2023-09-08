#include "config.h"
#include "displaymanager.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>

#define OPTION(name, type, defaultValue, displayName)                 \
	type Configuration::name                          = defaultValue; \
	void (*Configuration::handler_##name)(type & arg) = nullptr;
#include "config_options.h"

const char *configFileName = "/config.json";

void Configuration::load() {
	File configFile = SPIFFS.open(configFileName, "r");
	if(!configFile) {
		Serial.println("Failed to open config file");
		return;
	}

	size_t size = configFile.size();
	if(size > 1024) {
		Serial.println("Config file size is too large");
		return;
	}

	DynamicJsonDocument  doc(1024);
	DeserializationError error = deserializeJson(doc, configFile);
	if(error) {
		Serial.printf("Failed to parse config file: %s\n", error.c_str());
		return;
	}

#define OPTION(name, type, defaultValue, displayName) \
	if(doc.containsKey(#name)) \
		name = doc[#name].as<type>();
#include "config_options.h"

	configFile.close();
}

void Configuration::store() {
	StaticJsonDocument<1024> doc;
#define OPTION(name, type, defaultValue, displayName) doc[#name] = get##name();
#include "config_options.h"
	File configFile = SPIFFS.open(configFileName, "w");
	if(!configFile) {
		DisplayManager::printScrollingText("Error saving config!");
		return;
	}
	serializeJson(doc, configFile);
	configFile.close();
}