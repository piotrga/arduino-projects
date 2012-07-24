#include <avr/interrupt.h>  
#include <avr/io.h>
boolean state_13;
long counter;

//Timer2 overflow interrupt vector handler, called (16,000,000/256)/256 times per second
ISR(TIMER2_OVF_vect) {
  //let 10 indicates interrupt fired
  counter++;
//  if (counter % 16 == 0){
    digitalWrite(13, !state_13);
    state_13 = !state_13;
//  }
};  

void setup() {
  state_13 = true;
  counter = 0;
  pinMode(13,OUTPUT);
//  pinMode(10,OUTPUT);

  //Timer2 Settings: Timer Prescaler /1024, WGM mode 0
  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20 ;

  //Timer2 Overflow Interrupt Enable  
  TIMSK2 = 1<<TOIE2;

  //reset timer
  TCNT2 = 0;

  //led 9 indicate ready
  digitalWrite(13,true);
}

void loop() {
}
