#pragma once

#include <stdlib.h>

struct Scheduler {
	struct Schedulable;

  private:
	Schedulable  *tasks;
	int           taskCount;
	unsigned long lastRunMills;

	void schedule(Schedulable *a) {
		tasks =
		    (Schedulable *)realloc(tasks, sizeof(Schedulable) * ++taskCount);
		tasks[taskCount - 1]           = *a;
		tasks[taskCount - 1].remaining = a->gap_millis;
	}

  public:
	Scheduler() : tasks(NULL), taskCount(0), lastRunMills(0) {}
	~Scheduler() { free(tasks); }
	struct Schedulable {
		unsigned long gap_millis;
		unsigned long remaining;
		Scheduler    *scheduler;

		void (*task)();

		void perform(void (*t)()) {
			task = t;
			scheduler->schedule(this);
		}
	};
	struct IntervalSchedulable {
		unsigned long interval;
		Scheduler    *scheduler;
		Schedulable   second(unsigned long sec = 1) {
			  return Schedulable{interval * sec * 1000, 0, scheduler, nullptr};
		}
		Schedulable minute() { return second(60); }
		Schedulable hour() { return second(3600); }
		Schedulable day() { return second(86400); }
	};

	IntervalSchedulable every(unsigned long interval = 1) {
		return IntervalSchedulable{interval, this};
	}

	void run() {
		unsigned long now     = millis();
		unsigned long elapsed = (now - lastRunMills);
		lastRunMills          = now;
		for(int i = 0; i < taskCount; i++) {
			if(elapsed >= tasks[i].remaining) {
				// Serial.printf("Running task %d\n", i);
				tasks[i].task();
				tasks[i].remaining = tasks[i].gap_millis;
			} else {
				tasks[i].remaining -= elapsed;
			}
		}
	}
};