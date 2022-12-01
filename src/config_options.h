#ifndef OPTION
#define OPTION(name, type, defaultValue)
#endif

OPTION(DisplayState, bool, true)
OPTION(APName, std::string, "ACAcessPoint")
OPTION(APPassword, std::string, "RouteR1913")
OPTION(NTPServer, std::string, "pool.ntp.org")
OPTION(OffsetHours, int, 5)
OPTION(OffsetMinutes, int, 30)

#undef OPTION