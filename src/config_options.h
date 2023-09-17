#ifndef OPTION
#define OPTION(name, type, defaultValue, displayName)
#endif

OPTION(APName, String, "Wokwi-GUEST", "Access Point Name")
OPTION(APPassword, String, "", "Access Point Password")
OPTION(NTPServer, String, "pool.ntp.org", "NTP Server")
OPTION(OffsetHours, int, 5, "GMT Offset Hours")
OPTION(OffsetMinutes, int, 30, "GMT Offset Minutes")
OPTION(Username, String, "Subhranil", "Your name")
OPTION(Latitude, float, 22.5626, "Latitude")
OPTION(Longitude, float, 88.363, "Longitude")
OPTION(DisplayState, bool, true, "Display On")
OPTION(TempFahrenhite, bool, false, "Temperature in °F")
OPTION(Clock24, bool, false, "24 Hour Clock")
OPTION(ShowOutdoorTemperature, bool, true, "Show Outdoor Temperature")

#undef OPTION