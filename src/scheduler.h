#pragma once

#include <Arduino.h>
#include <stdlib.h>

struct Scheduler {
	struct Schedulable;

  private:
	static Schedulable  *tasks;
	static int           taskCount;
	static unsigned long taskID;
	static unsigned long lastRunMills;

	static unsigned long schedule(Schedulable *a) {
		tasks =
		    (Schedulable *)realloc(tasks, sizeof(Schedulable) * ++taskCount);
		tasks[taskCount - 1]               = *a;
		tasks[taskCount - 1].remaining     = a->gap_millis;
		return tasks[taskCount - 1].taskID = Scheduler::taskID++;
	}

	static void removeTask(int idx) {
		taskCount--;
		for(int i = idx; i < taskCount; i++) {
			tasks[i] = tasks[i + 1];
		}
		tasks = (Schedulable *)realloc(tasks, sizeof(Schedulable) * taskCount);
	}

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
		Schedulable   second(unsigned long sec = 1) {
			  return Schedulable{interval * sec * 1000, 0, 0, false, nullptr};
		}
		Schedulable minute() { return second(60); }
		Schedulable hour() { return second(3600); }
		Schedulable day() { return second(86400); }
	};

	static IntervalSchedulable every(unsigned long interval = 1) {
		return IntervalSchedulable{interval};
	}

	static void unschedule(unsigned long taskID) {
		for(int i = 0; i < taskCount; i++) {
			if(tasks[i].taskID == taskID) {
				removeTask(i);
				return;
			}
		}
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
					removeTask(i);
					i--;
				}
			} else {
				tasks[i].remaining -= elapsed;
			}
		}
	}
};