#ifndef OPTION
#define OPTION(name, type, defaultValue, displayName)
#endif

OPTION(APName, String, "ACAcessPoint", "Access Point Name")
OPTION(APPassword, String, "RouteR1913", "Access Point Password")
OPTION(NTPServer, String, "pool.ntp.org", "NTP Server")
OPTION(OffsetHours, int, 5, "GMT Offset Hours")
OPTION(OffsetMinutes, int, 30, "GMT Offset Minutes")
OPTION(Username, String, "Subhranil", "Your name")
OPTION(DisplayState, bool, true, "Display On")

#undef OPTION