#include "alarm.h"

Timer_t Alarm::timers[] = {};
int Alarm::alocatedTimers = 0;

void Alarm::repeatTimer(long interval, int repetitions, void (*callback)(void *), void *data , bool addStartDelay, long startDelay){
	Timer_t *timer = (Timer_t*)malloc(sizeof(Timer_t));
	timer->lastTime = millis();
	timer->interval = interval;
	timer->repetitions = repetitions;
	timer->running = true;
	timer->callback = callback;
	timer->data = data;

	if(addStartDelay){
#ifdef DEBUG
		Serial.println("Start delay on!");
#endif
		if(startDelay == -1){
			randomSeed(analogRead(A5));
			startDelay = random(interval/100);
#ifdef DEBUG
			Serial.print("startDelay: ");
			Serial.println(startDelay);
#endif
		}
		oneTimeTimer(startDelay, Alarm::addTimer, timer);
	}else{
		addTimer(timer);
	}
};

void Alarm::infiniteTimer(long interval, void (*callback)(void *), void *data , bool addStartDelay, long startDelay){
#ifdef DEBUG
	Serial.println("Infinite timer scheduled.");
#endif
	repeatTimer(interval, -1,  callback, data, addStartDelay, startDelay);
};

void Alarm::oneTimeTimer(long interval, void (*callback)(void *), void *data){
#ifdef DEBUG
	Serial.println("One time timer scheduled.");
#endif	
	repeatTimer(interval, 1,  callback, data);
};

void Alarm::checkTimers(){
	int i;
	long now = millis();
	for (i = 0; i < MAX_TIMERS; ++i)
	{
		if(Alarm::timers[i].running == true){
			if(Alarm::timers[i].lastTime + Alarm::timers[i].interval < now){
#ifdef DEBUG
				Serial.print("Last Time: ");
				Serial.println(Alarm::timers[i].lastTime);
				Serial.print("Last Time + interval: ");
				Serial.println(Alarm::timers[i].lastTime + Alarm::timers[i].interval);
				Serial.print("Now: ");
				Serial.println(now);
#endif
				Alarm::timers[i].lastTime = now;
				if(Alarm::timers[i].repetitions!=-1){
					Alarm::timers[i].repetitions--;
					if(Alarm::timers[i].repetitions <= 0){
						Alarm::timers[i].running = false;
						Alarm::alocatedTimers--;
#ifdef DEBUG
						Serial.println("Timer stoped");
						Serial.print("Alocated Timers: ");
						Serial.println(Alarm::alocatedTimers);
#endif
					}
				}
#ifdef DEBUG
				Serial.println("Calling callback.");
#endif	
				Alarm::timers[i].callback(Alarm::timers[i].data);
#ifdef DEBUG
				Serial.println("Callback called.");
#endif	
			}
		}
	}
}

void Alarm::addTimer(void *newTimer){
	Timer_t *t = (Timer_t*) newTimer;
	t->lastTime = millis();
#ifdef DEBUG
	Serial.print("lastTime: ");
	Serial.print(t->lastTime);
	Serial.print(" interval: ");
	Serial.print(t->interval);
	Serial.print(" repetitions: ");
	Serial.print(t->repetitions);
	Serial.print(" running: ");
	Serial.println(t->running);
#endif
	
	// find free timer 
	int i;
	for (i = 0;i < MAX_TIMERS; i++)
	{
		if(!Alarm::timers[i].running)
		{
			Alarm::timers[i] = *t;
			Alarm::alocatedTimers++;
			break;
		}
	}
	
#ifdef DEBUG
	Serial.println("Timer added!");
	Serial.print("Alocated Timers: ");
	Serial.println(Alarm::alocatedTimers);
#endif
}
