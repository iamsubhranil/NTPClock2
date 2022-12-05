#include "scheduler.h"

Scheduler::Schedulable *Scheduler::tasks        = nullptr;
int                     Scheduler::taskCount    = 0;
unsigned long           Scheduler::lastRunMills = 0;