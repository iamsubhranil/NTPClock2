#pragma once

struct WeatherManager {
    struct Weather {
        float temperature;
        float windspeed;
        int weathercode;
        int is_day;
    };

    static Weather currentWeather;

    static void init();
    static void sync();
};