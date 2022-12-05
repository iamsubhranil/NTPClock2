#pragma once

#include <Arduino.h>

struct LEDManager {
	static const int BLUE = 2;

	static void init() { pinMode(BLUE, OUTPUT); }
	static void blink(int led);
	static void off(int led);
	static void on(int led);
};