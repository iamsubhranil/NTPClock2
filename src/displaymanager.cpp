#include "displaymanager.h"
#include "config.h"

// Defining size, and output pins
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 7
#define CS_PIN 5

MD_Parola DisplayManager::display =
    MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void DisplayManager::init() {
	display.begin(0);
	display.setIntensity(0);
	display.displayClear();

	display.setTextAlignment(PA_CENTER);
	display.print("Hello!");
	delay(1000);

	Configuration::onDisplayStateChange(
	    [](bool &state) { display.displayShutdown(!state); });
}