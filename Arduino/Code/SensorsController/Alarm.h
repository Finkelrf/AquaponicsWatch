#ifndef ALARM_H	
#define ALARM_H

#include "common.h"

#define MAX_TIMERS 10
typedef struct Timer { 
    long lastTime;
    long interval;
    int repetitions;
    bool running;
    void (*callback)();
} Timer_t;

class Alarm
{
	Timer_t timers[MAX_TIMERS];
	int alocatedTimers = 0;
	void addTimer(Timer_t newTimer);

public:
	Alarm();
	void repeatTimer(long interval, int numberOfRepetitions, void (*callback)(void));
	void infiniteTimer(long interval, void (*callback)(void));
	void oneTimeTimer(long interval, void (*callback)(void));

	void checkTimers();
	long lastTime;
	long interval;
	long repetitionsLeft;
};



#endif
