#pragma once

#include <ArduinoJson.h>

struct Time {
    int hours;
    int minutes;

    Time(int a, int b) : hours(a), minutes(b) {}
    Time() : hours(0), minutes(0) {}

    bool operator==(const Time &a) {
        return a.hours == hours && a.minutes == minutes;
    }

    String toString() const {
        char buf[10];
        snprintf(buf, sizeof(buf), "%02d:%02d", hours, minutes);
        printf("%s", buf);
        return String(buf);
    }

    static Time fromString(const char* s) {
        Time t;
        sscanf(s, "%d:%d", &t.hours, &t.minutes);
        return t;
    }

    static Time fromString(const String &s) {
        return fromString(s.c_str());
    }

    static bool validate(const char *str) {
        return true;
    }
};

struct Timer {
    static void init();
};