#include "BMPManager.h"
#include "config.h"
#include "weathermanager.h"

#include <Adafruit_BMP280.h>
#include <Wire.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

static Adafruit_BMP280 bmpSensor;
static bool sensorFound = false;

void BMPManager::init() {
	Configuration::onTempFahrenhiteChange([](bool &arg) {});
	Configuration::onShowOutdoorTemperatureChange([](bool &arg) {});
	if(!Configuration::getShowOutdoorTemperature()) {
		scanSensors();
	}
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
	else {
		Serial.printf("BMP sensor found: %x\n", bmpSensor.sensorID());

		bmpSensor.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
				Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
				Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
				Adafruit_BMP280::FILTER_X16,      /* Filtering. */
				Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
		
		sensorFound = true;
	}
}

float BMPManager::getTemperature() {
    float temp = 0;
	if(Configuration::getShowOutdoorTemperature()) {
		temp = WeatherManager::currentWeather.temperature;
	} else {
		if(!sensorFound) {
			scanSensors();
		}
		temp = bmpSensor.readTemperature();
	}
	if(Configuration::getTempFahrenhite()) {
		temp = (temp * 9)/5 + 32;
	}
	return temp;
}

const char* BMPManager::getTemperatureString() {
	static char tempString[50] = {0};
	snprintf(tempString, 50, "%d\x90%s", (int)getTemperature(), Configuration::getTempFahrenhite() ? "F" : "c");
	return tempString;
}

float BMPManager::getAltitude() {
    return bmpSensor.readAltitude();
}

float BMPManager::getPressure() {
    return bmpSensor.readPressure();
}