#include "BMPManager.h"
#include <Adafruit_BMP280.h>
#include <Wire.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmpSensor;

void BMPManager::init() {
	scanSensors();
    
    bmpSensor.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
            Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
            Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
            Adafruit_BMP280::FILTER_X16,      /* Filtering. */
            Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void BMPManager::scanSensors() {
	Wire.begin();
    auto status = bmpSensor.begin(0x76);
    if(!status) {
        Serial.printf("Unable to find BMP sensor! Error: %x\n", bmpSensor.sensorID());
		byte error, address;
		int nDevices;
		Serial.println("Scanning...");
		nDevices = 0;
		for(address = 1; address < 127; address++ ) {
			Wire.beginTransmission(address);
			error = Wire.endTransmission();
			if (error == 0) {
				Serial.print("I2C device found at address 0x");
				if (address<16) {
					Serial.print("0");
				}
				Serial.println(address,HEX);
				nDevices++;
			}
			else if (error==4) {
				Serial.print("Unknow error at address 0x");
				if (address<16) {
					Serial.print("0");
				}
				Serial.println(address,HEX);
			}    
		}
		if (nDevices == 0) {
			Serial.println("No I2C devices found\n");
		}
		else {
			Serial.println("done\n");
		}
	}
	else
		Serial.printf("BMP sensor found: %x\n", bmpSensor.sensorID());
}

float BMPManager::getTemperature() {
    return bmpSensor.readTemperature();
}

float BMPManager::getAltitude() {
    return bmpSensor.readAltitude();
}

float BMPManager::getPressure() {
    return bmpSensor.readPressure();
}