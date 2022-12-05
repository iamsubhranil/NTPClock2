#include "ledmanager.h"

#include <esp_task.h>

struct BlinkingLED {
	int          led;
	TaskHandle_t task;
};

BlinkingLED blinkingLEDs[100] = {0};
int         blinkingLEDCount  = 0;

void blinker(void *led) {
	while(true) {
		digitalWrite((int)led, HIGH);
		vTaskDelay(500);
		digitalWrite((int)led, LOW);
		vTaskDelay(500);
	}
}

void LEDManager::blink(int led) {
	TaskHandle_t task;
	xTaskCreate(blinker, "blinker", 2048, (void *)led, 1, &task);
	blinkingLEDs[blinkingLEDCount++] = {led, task};
}

void LEDManager::on(int led) {
	for(int i = 0; i < blinkingLEDCount; i++) {
		if(blinkingLEDs[i].led == led) {
			vTaskDelete(blinkingLEDs[i].task);
			blinkingLEDs[i] = blinkingLEDs[--blinkingLEDCount];
			break;
		}
	}
	digitalWrite(led, HIGH);
}

void LEDManager::off(int led) {
	for(int i = 0; i < blinkingLEDCount; i++) {
		if(blinkingLEDs[i].led == led) {
			vTaskDelete(blinkingLEDs[i].task);
			blinkingLEDs[i] = blinkingLEDs[--blinkingLEDCount];
			break;
		}
	}
	digitalWrite(led, LOW);
}