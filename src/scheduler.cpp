#include "scheduler.h"

Scheduler::Schedulable *Scheduler::tasks            = nullptr;
int                     Scheduler::taskCount        = 0;
unsigned long           Scheduler::lastRunMills     = 0;
unsigned long           Scheduler::taskID           = 0;
QueueHandle_t           Scheduler::scheduleQueue    = nullptr;
QueueHandle_t           Scheduler::unscheduleQueue  = nullptr;
int                     Scheduler::runningOnCore    = 0;
int                     Scheduler::granularityMills = 1000;
const static int        schedulerPriority           = 1;

void Scheduler::insertTask(Schedulable &a) {
	tasks = (Schedulable *)realloc(tasks, sizeof(Schedulable) * ++taskCount);
	tasks[taskCount - 1]           = a;
	tasks[taskCount - 1].remaining = a.gap_millis;
	if(a.gap_millis < granularityMills) {
		granularityMills = a.gap_millis;
	}
}

void Scheduler::removeTask(int idx) {
	taskCount--;
	for(int i = idx; i < taskCount; i++) {
		tasks[i] = tasks[i + 1];
	}
	tasks = (Schedulable *)realloc(tasks, sizeof(Schedulable) * taskCount);
}

unsigned long Scheduler::schedule(Schedulable *a) {
	a->taskID = Scheduler::taskID++;
	if(xPortGetCoreID() != runningOnCore) {
		// if we are not running on the core the scheduler is running on, we
		// enqueue the schedule, which will be performed after next run()
		xQueueSend(scheduleQueue, a, portMAX_DELAY);
	} else {
		insertTask(*a);
	}
	return a->taskID;
}

void Scheduler::unschedule(unsigned long taskID) {
	if(xPortGetCoreID() != runningOnCore) {
		// if we are not running on the core the scheduler is running on, we
		// enqueue the unschedule, which will be performed before next run()
		xQueueSend(unscheduleQueue, &taskID, portMAX_DELAY);
		return;
	}
	for(int i = 0; i < taskCount; i++) {
		if(tasks[i].taskID == taskID) {
			removeTask(i);
			return;
		}
	}
}

void Scheduler::run() {
	unsigned long now     = millis();
	unsigned long elapsed = (now - lastRunMills);
	lastRunMills          = now;
	for(int i = 0; i < taskCount; i++) {
		if(elapsed >= tasks[i].remaining) {
			// Serial.printf("Running task %d\n", i);
			tasks[i].task();
			tasks[i].remaining = tasks[i].gap_millis;
			if(tasks[i].oneshot) {
				// Serial.printf("Removing task %d\n", i);
				removeTask(i);
				i--;
			}
		} else {
			tasks[i].remaining -= elapsed;
		}
	}
}

void Scheduler::addQueuedTasks() {
	while(uxQueueMessagesWaiting(scheduleQueue) > 0) {
		Schedulable task;
		if(xQueueReceive(scheduleQueue, &task, 0)) {
			insertTask(task);
		}
	}
}

void Scheduler::removeQueuedTasks() {
	while(uxQueueMessagesWaiting(unscheduleQueue) > 0) {
		unsigned long taskID;
		if(xQueueReceive(unscheduleQueue, &taskID, 0)) {
			unschedule(taskID);
		}
	}
}

void Scheduler::begin() {
	xTaskCreatePinnedToCore(
	    [](void *param) {
		    while(true) {
			    removeQueuedTasks();
			    run();
			    addQueuedTasks();
			    delay(granularityMills);
		    }
	    },
	    "Scheduler", 10000, NULL, schedulerPriority, NULL, runningOnCore);
}

void Scheduler::init() {
	lastRunMills    = millis();
	runningOnCore   = xPortGetCoreID();
	scheduleQueue   = xQueueCreate(10, sizeof(Schedulable));
	unscheduleQueue = xQueueCreate(10, sizeof(unsigned long));
}