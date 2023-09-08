#include "weathermanager.h"
#include "wifimanager.h"
#include "config.h"
#include "animation.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>

char weatherAPI[110] = {0};
const char *weatherAPIFormat = "https://api.open-meteo.com/v1/forecast?latitude=%.3f&longitude=%.3f&current_weather=true&timezone=auto";
WeatherManager::Weather WeatherManager::currentWeather = {0};
bool syncing = false;

void WeatherManager::init() {
    Animation::addFrame(3, [](char *text, int textSize, int tick) {
        char weatherSign = 193;
        const char* weatherText = "Possibly";
        switch(currentWeather.weathercode) {
            case 0: weatherSign = currentWeather.is_day ? 195 : 193; weatherText = "Clear Sky"; break;
            case 1: weatherSign = currentWeather.is_day ? 196 : 194; weatherText = "Partly Clr"; break;
            case 2:
            case 3: weatherSign = 197; weatherText = "Cloudy"; break;
            case 45:
            case 48:
                weatherSign = 202; weatherText = "Foggy"; break;
            case 51:
            case 53:
            case 55:
            case 56:
            case 57:
                weatherSign = 203; weatherText = "Drizzle"; break;
            case 61:
            case 66:
            case 80:
                weatherSign = 198; weatherText = "Light Rain"; break;
            case 63: weatherSign = 199; weatherText = "Mod. Rain"; break;
            case 65:
            case 67:
            case 82:
                weatherSign = 200; weatherText = "Heavy Rain"; break;
            case 71:
            case 73:
            case 75:
            case 77:
            case 85:
            case 86:
                weatherSign = 204; weatherText = "Snow"; break;
            case 95:
            case 96:
            case 99:
                weatherSign = 201; weatherText = "Thunder"; break;
        }
        text[0] = weatherSign;
        snprintf(text + 1, textSize - 1, " %s", weatherText);
    });
}

void WeatherManager::sync() {
    if(!WiFiManager::isConnected() || syncing)
        return;
    syncing = true;
    HTTPClient httpClient;
    snprintf(weatherAPI, sizeof(weatherAPI), weatherAPIFormat, Configuration::getLatitude(), Configuration::getLongitude());
    Serial.printf("Weather API: %s", weatherAPI);
    httpClient.begin(weatherAPI);
    int response = httpClient.GET();
    if(response != 200) {
        Serial.printf("Weather API failed: %d\n", response);
        syncing = false;
        return;
    }

    String payload = httpClient.getString();
    DynamicJsonDocument document(1024);
    DeserializationError error = deserializeJson(document, payload);

    if(error) {
        Serial.printf("Failed to parse response: %s %d %d\n", error.c_str(), httpClient.getSize(), esp_get_free_heap_size());
        syncing = false;
        return;
    }

    currentWeather.temperature = document["current_weather"]["temperature"];
    currentWeather.weathercode = document["current_weather"]["weathercode"];
    currentWeather.windspeed = document["current_weather"]["windspeed"];
    currentWeather.is_day = document["current_weather"]["is_day"];

    syncing = false;
}