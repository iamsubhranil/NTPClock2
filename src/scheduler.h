#pragma once

#include <Arduino.h>
#include <stdlib.h>

struct Scheduler {
	struct Schedulable;

  private:
	static Schedulable  *tasks;
	static int           taskCount;
	static unsigned long lastRunMills;

	static void schedule(Schedulable *a) {
		tasks =
		    (Schedulable *)realloc(tasks, sizeof(Schedulable) * ++taskCount);
		tasks[taskCount - 1]           = *a;
		tasks[taskCount - 1].remaining = a->gap_millis;
	}

  public:
	struct Schedulable {
		unsigned long gap_millis;
		unsigned long remaining;
		bool          oneshot;

		void (*task)();

		void perform(void (*t)(), bool once = false) {
			task    = t;
			oneshot = once;
			Scheduler::schedule(this);
		}
	};
	struct IntervalSchedulable {
		unsigned long interval;
		Schedulable   second(unsigned long sec = 1) {
			  return Schedulable{interval * sec * 1000, 0, false, nullptr};
		}
		Schedulable minute() { return second(60); }
		Schedulable hour() { return second(3600); }
		Schedulable day() { return second(86400); }
	};

	static IntervalSchedulable every(unsigned long interval = 1) {
		return IntervalSchedulable{interval};
	}

	static void run() {
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
					taskCount--;
					for(int j = i; j < taskCount; j++) {
						tasks[j] = tasks[j + 1];
					}
					tasks = (Schedulable *)realloc(tasks, sizeof(Schedulable) *
					                                          taskCount);
					i--;
				}
			} else {
				tasks[i].remaining -= elapsed;
			}
		}
	}
};