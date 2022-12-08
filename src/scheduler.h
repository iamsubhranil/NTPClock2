#pragma once

#include <Arduino.h>

struct Scheduler {
	struct Schedulable;

  private:
	static Schedulable  *tasks;
	static int           taskCount;
	static unsigned long taskID;
	static unsigned long lastRunMills;
	static QueueHandle_t scheduleQueue;
	static QueueHandle_t unscheduleQueue;
	static int           runningOnCore;
	static int
	    granularityMills; // minimum time between runs, which is also the lowest
	                      // possible interval specified by any task

	static void          insertTask(Schedulable &a);
	static unsigned long schedule(Schedulable *a);
	static void          removeTask(int idx);
	static void          run();
	static void          addQueuedTasks();
	static void          removeQueuedTasks();

  public:
	struct Schedulable {
		unsigned long gap_millis;
		unsigned long remaining;
		unsigned long taskID;
		bool          oneshot;

		void (*task)();

		unsigned long perform(void (*t)(), bool once = false) {
			task    = t;
			oneshot = once;
			return Scheduler::schedule(this);
		}
	};
	struct IntervalSchedulable {
		unsigned long interval;
		Schedulable   millisecond(unsigned long ms = 1) {
            return Schedulable{interval * ms, 0, 0, false, nullptr};
		}
		Schedulable second(unsigned long sec = 1) {
			return millisecond(1000 * sec);
		}
		Schedulable minute() { return second(60); }
		Schedulable hour() { return second(3600); }
		Schedulable day() { return second(86400); }
	};

	static IntervalSchedulable every(unsigned long interval = 1) {
		return IntervalSchedulable{interval};
	}

	static void unschedule(unsigned long taskID);
	static void begin();
	static void init();
};