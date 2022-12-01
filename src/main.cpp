#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <SPI.h>
#include <WiFi.h>
#include <time.h>

#include "clock.h"
#include "scheduler.h"
#include "server.h"

// Uncomment according to your hardware type
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
//#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

// Defining size, and output pins
#define MAX_DEVICES 7
#define CS_PIN 5

const long gmOffset_hours = 5;
const long gmOffset_mins  = 30;

const char *ntpServer          = "pool.ntp.org";
const long  gmtOffset_sec      = gmOffset_hours * 3600 + gmOffset_mins * 60;
const int   daylightOffset_sec = 0;

MD_Parola Display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const char *SSID     = "ACAcessPoint";
const char *PASSWORD = "RouteR1913";

Scheduler scheduler;

void syncTime() {
	configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void setup() {

	Serial.begin(9600);

	Display.begin(0);
	// Display.setZone(0, 0, 2);
	// Display.setZone(1, 3, 6);
	Display.setIntensity(0);
	Display.displayClear();

	WiFi.mode(WIFI_STA);

	WiFi.begin(SSID, PASSWORD);

	Display.setTextAlignment(PA_CENTER);
	Display.print("Hello!");
	while(WiFi.status() != WL_CONNECTED) {
	}

	ServerManager::init();

	Display.displayClear();
	Display.print("Starting..");
	syncTime();

	scheduler.every(30).minute().perform([]() { syncTime(); });
	scheduler.every().second().perform([]() {
		Display.displayClear();

		struct tm timeinfo;
		if(!getLocalTime(&timeinfo)) {
			Display.print("Bad Time!");
		} else {
			// Serial.println("Time retrieved..");
			Clock::show(Display, timeinfo);
		}
	});

	// Display.setTextAlignment(0, PA_CENTER);
	// Display.setTextAlignment(1, PA_CENTER);
}

void loop() {
	scheduler.run();
	delay(200);
}