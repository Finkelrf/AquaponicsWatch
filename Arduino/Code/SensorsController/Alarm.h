#ifndef ALARM_H	
#define ALARM_H

#include "common.h"

#define AIR_TEMPERATURE_INTERVAL 60000
#define AIR_HUMIDITY_INTERVAL 60000
#define FISHTANK_TEMP_INTERVAL 60000
#define RAIN_SENSOR_INTERVAL 60000
#define FEED_INTERVAL 21600000

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
	static void repeatTimer(long interval, int numberOfRepetitions, void (*callback)(void *), void *data = NULL, bool addStartDelay = false, long startDelay = -1);
	static void infiniteTimer(long interval, void (*callback)(void *), void *data = NULL, bool addStartDelay = false, long startDelay = -1);
	static void oneTimeTimer(long interval, void (*callback)(void *), void *data = NULL);

	static void checkTimers();
};



#endif
