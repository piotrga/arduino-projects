#include "WDT.h"
#include "Clock.h"

EnergySaver saver(WDT_1024K);
Clock clock(0);

void setup(){
  disable_wdt_MCU_reset();
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  Serial.println("Start");
  saver.on_wake_up_call(on_wake_up);
}

void on_wake_up(){
  Serial.println("On wake up!");
  clock.updateTime();
}


void loop(){
  blink(200);
  saver.sleep_and_save_power();  

  unsigned time = clock.time();
  Serial.print(time, DEC);
  Serial.print(":");
  Serial.println("Wake up2!");
}

void blink(int time){
  digitalWrite(13, HIGH);   
  delay(time);              
  digitalWrite(13, LOW);
  delay(time);    
}



