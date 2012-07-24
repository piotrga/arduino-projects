#ifndef Clock_h
#define Clock_h

#include "WProgram.h"

class Clock{
public:
  Clock(long startTime);
  long time();
  void updateTime();
private:
  long _time;  
  unsigned lastMillis;
};

#endif