#include "storage.h"
#include "displaymanager.h"

bool Storage::initialized = false;

void Storage::init() {
	if(!SPIFFS.begin(true)) {
		Serial.printf("[" __FILE__ ":%d] Failed to mount SPIFFS", __LINE__);
	} else {
		initialized = true;
	}
}

bool Storage::isInitialized() {
	return initialized;
}

File Storage::open(const char *path, const char *mode) {
	return SPIFFS.open(path, mode);
}

bool Storage::exists(const char *path) {
	return SPIFFS.exists(path);
}

String Storage::read(const char *path) {
	File f = SPIFFS.open(path, "r");
	if(!f) {
		Serial.printf("[" __FILE__ ":%d] Failed to open file '%s' for reading",
		              __LINE__, path);
		return "";
	}
	return f.readString();
}

bool Storage::write(const char *path, const char *data) {
	File f = SPIFFS.open(path, "w");
	if(!f) {
		Serial.printf("[" __FILE__ ":%d] Failed to open file '%s' for writing",
		              __LINE__, path);
		return false;
	}
	f.print(data);
	return true;
}