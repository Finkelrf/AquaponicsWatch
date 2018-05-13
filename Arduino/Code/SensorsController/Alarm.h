#ifndef ALARM_H	
#define ALARM_H

#include "common.h"

#define AIR_TEMPERATURE_INTERVAL 5*1000//10*60*1000
#define AIR_HUMIDITY_INTERVAL 5*1000//10*60*1000
#define FISHTANK_WATER_TEMPERATURE_INTERVAL 5*1000/10*60*1000
#define RAIN_SENSOR_INTERVAL 5*1000//10*60*1000

#define MAX_TIMERS 10
typedef struct Timer { 
    long lastTime;
    long interval;
    int repetitions;
    bool running;
    void (*callback)(void *);
    void *data;
} Timer_t;

class Alarm
{

	static Timer_t timers[MAX_TIMERS];
	static int alocatedTimers;
	static void addTimer(void *newTimer);

public:
	Alarm();
	void repeatTimer(long interval, int numberOfRepetitions, void (*callback)(void *), void *data = NULL, bool addStartDelay = false, long startDelay = -1);
	void infiniteTimer(long interval, void (*callback)(void *), void *data = NULL, bool addStartDelay = false, long startDelay = -1);
	void oneTimeTimer(long interval, void (*callback)(void *), void *data = NULL);

	void checkTimers();
	long lastTime;
	long interval;
	long repetitionsLeft;
};



#endif
