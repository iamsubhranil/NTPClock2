#include "clock.h"

char Clock::timeStringBuff[50] = {0};

const char *Clock::formatStrings[3] = {"%I : %M %p", "%I   %M %p",
                                       "%a,  %b %e"};

int Clock::currentTick = 0;