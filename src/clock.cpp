#include "clock.h"

char Clock::timeStringBuff[50] = {0};

const char *Clock::formatStrings[5] = {"%I. %M. %S %p", "%I  %M  %S %p",
                                       "%H. %M. %S", "%H  %M  %S", "%a %e, "};

int Clock::currentTick = 0;