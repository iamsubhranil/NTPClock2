#pragma once

#include "config.h"
#include "displaymanager.h"
#include "BMPManager.h"
#include <time.h>

struct Clock {
	static char timeStringBuff[50];

	static const int clockDuration = 6;
	static const int dateDuration  = 3;

	static const int period = clockDuration + dateDuration;
	static int       currentTick;

	static const char *formatStrings[5];

	static void show(tm &timeinfo) {
		int idx;

		if(currentTick < clockDuration) {
			idx = currentTick % 2;
			if(Configuration::getClock24())
				idx += 2;
		} else {
			idx = 4;
		}

		// Serial.printf("tick: %d mode: %d idx: %d", currentTick, mode, idx);
		const char *fmt = formatStrings[idx];

		// Serial.println(fmt);
		size_t written = strftime(timeStringBuff, 50, fmt, &timeinfo);
		if(idx == 4) {
			snprintf(timeStringBuff + written, 50 - written, "%s", (int)BMPManager::getTemperatureString());
		}
		DisplayManager::clear();
		DisplayManager::print(timeStringBuff);

		currentTick++;
		currentTick = currentTick % period;
	}
};