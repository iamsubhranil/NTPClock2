#ifndef OPTION
#define OPTION(name, type, defaultValue)
#endif

OPTION(DisplayState, bool, true)
OPTION(APName, String, "ACAcessPoint")
OPTION(APPassword, String, "RouteR1913")
OPTION(NTPServer, String, "pool.ntp.org")
OPTION(OffsetHours, int, 5)
OPTION(OffsetMinutes, int, 30)

#undef OPTION