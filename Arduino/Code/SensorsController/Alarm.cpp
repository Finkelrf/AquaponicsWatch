#include "alarm.h"

Alarm::Alarm() {
   int i;
   for (i = 0; i < MAX_TIMERS; ++i)
   {
   	timers[i].running = false;
   }
}

void Alarm::repeatTimer(long interval, int repetitions, void (*callback)(void), bool addSalt){
	long salt = 0;
	if(addSalt){
		randomSeed(analogRead(A5));
		salt = random(interval/100);
		Serial.print("salt: ");
		Serial.println(salt);
	}
	Timer_t timer;
	timer.lastTime = millis();
	timer.interval = interval;
	timer.repetitions = repetitions+salt;
	timer.running = true;
	timer.callback = callback;
	addTimer(timer);

};

void Alarm::oneTimeTimer(long interval, void (*callback)(void)){
	Timer_t timer;
	timer.lastTime = millis();
	timer.interval = interval;
	timer.repetitions = 1;
	timer.running = true;
	timer.callback = callback;
	addTimer(timer);

};

void Alarm::infiniteTimer(long interval, void (*callback)(void), bool addSalt){
	long salt = 0;
	if(addSalt){
		randomSeed(analogRead(A5));
		salt = random(interval/100);
		Serial.print("salt: ");
		Serial.println(salt);
	}
	Timer_t timer;
	timer.lastTime = millis();
	timer.interval = interval + salt;
	timer.repetitions = -1;
	timer.running = true;
	timer.callback = callback;
	addTimer(timer);

};

void Alarm::checkTimers(){
	int i;
	long now = millis();
	for (i = 0; i < alocatedTimers; ++i)
	{
		if(timers[i].running == true){
			if(timers[i].lastTime + timers[i].interval < now){
#ifdef DEBUG
				Serial.print("Last Time: ");
				Serial.println(timers[i].lastTime);
				Serial.print("Last Time + interval: ");
				Serial.println(timers[i].lastTime + timers[i].interval);
				Serial.print("Now: ");
				Serial.println(now);
#endif
				timers[i].lastTime = now;
				if(timers[i].repetitions!=-1){
					timers[i].repetitions--;
					if(timers[i].repetitions <= 0){
						timers[i].running = false;
	#ifdef DEBUG
						Serial.println("Timer stoped");
	#endif
					}
				}
				timers[i].callback();
			}
		}
	}
}

void Alarm::addTimer(Timer_t newTimer){
	timers[alocatedTimers++] = newTimer;
}
