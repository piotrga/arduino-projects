#include "WProgram.h"
#include "Clock.h"

Clock::Clock(long startTime){
  _time = startTime;
  lastMillis = millis();
}

long Clock::time(){
  updateTime();
  return _time;
}

void Clock::updateTime(){
  unsigned currentMillis = millis();
  if (currentMillis != lastMillis){
    _time += currentMillis - lastMillis;
    lastMillis = currentMillis;
  }
}
