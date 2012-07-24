#include <WProgram.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#define byte unsigned char

// Documentation of WDT can be found in section 11.9 of http://www.atmel.com/dyn/resources/prod_documents/doc8271.pdf


long timeSleep = 0;  // total time due to sleep
float calibv = 0.93; // ratio of real clock with WDT clock
volatile byte isrcalled = 0;  // WDT vector flag

// Internal function: Start watchdog timer
// wdp - Prescale mask
void WDT_On (byte wdp)
{
 byte wdtcsr = wdp | (1<<WDIE); // enable wdt interrupt(WDIE)
 wdtcsr = wdtcsr & ~(1<<WDE);  // disable reset by wdt(WDE)
 cli();
 wdt_reset(); // restarts the timer in wdt
 set_WDTCSR(wdtcsr);
 sei();
}

void set_WDTCSR(ps){
 // The sequence for clearing WDE and changing time-out configuration is as follows:
 //	1. In the same operation, write a logic one to the Watchdog change enable bit (WDCE) and WDE. A logic one must be written to WDE regardless of the previous value of the WDE bit.
 //	2. Within the next four clock cycles, write the WDE and Watchdog prescaler bits (WDP) as desired, but with the WDCE bit cleared. This must be done in one operation.
 WDTCSR |= (1<<WDCE) | (1<<WDE);
 WDTCSR = ps;
}

void disable_wdt_MCU_reset(){
   // If the Watchdog is accidentally enabled, for example by a runaway pointer or brown-out condition,
   // the device will be reset and the Watchdog Timer will stay enabled. If the code is not set up to handle
   // the Watchdog, this might lead to an eternal loop of time-out resets. To avoid this situation,
   // the application software should always clear the Watchdog System Reset Flag (WDRF) and the WDE control
   // bit in the initialization routine, even if the Watchdog is not in use.
MCUSR &= ~(1<<WDRF);
}

// Internal function.  Stop watchdog timer
void WDT_Off() {
 cli();
 wdt_reset();
 disable_wdt_MCU_reset();
 set_WDTCSR(0); // disable both interrupt(WDIE) and reset(WDE) by wdt
 sei();
}

// Calibrate watchdog timer with millis() timer(timer0)
void calibrate() {
 // timer0 continues to run in idle sleep mode
 set_sleep_mode(SLEEP_MODE_IDLE);
 long tt1=millis();
 doSleep(256);
 long tt2=millis();
 calibv = 256.0/(tt2-tt1);
}

// Estimated millis is real clock + calibrated sleep time
long estMillis() {
 return millis()+timeSleep;
}

// Delay function
void sleepCPU_delay(long sleepTime) {
 ADCSRA &= ~(1<<ADEN);  // adc off
 PRR = 0xEF; // modules off

 set_sleep_mode(SLEEP_MODE_PWR_DOWN);
 int trem = doSleep(sleepTime*calibv);
 timeSleep += (sleepTime-trem);

 PRR = 0x00; //modules on
 ADCSRA |= (1<<ADEN);  // adc on
}

// Converts Prescale Select to WDP bits. See table 11-2
byte toWDP(ps){
return (ps & 0x08 ? (1<<WDP3) : 0x00) | (ps & 0x07);
}

// internal function.
int doSleep(long timeRem) {
 byte WDTps = 9;  // WDT Prescaler value, 9 = 8192ms

 isrcalled = 0;
 sleep_enable();
 while(timeRem > 0) {
   //work out next prescale unit to use
   while ((0x10<<WDTps) > timeRem && WDTps > 0) {
     WDTps--;
   }
   // send prescaler mask to WDT_On
   WDT_On(toWDP(WDTps));
   isrcalled=0;
   while (isrcalled==0) {
     // turn bod off
     MCUCR |= (1<<BODS) | (1<<BODSE);
     MCUCR &= ~(1<<BODSE);  // must be done right before sleep
     sleep_cpu();  // sleep here
   }
   // calculate remaining time
   timeRem -= (0x10<<WDTps);
 }
 sleep_disable();
 return timeRem;
}

// wdt int service routine
ISR(WDT_vect) {
 WDT_Off();
 isrcalled=1;
}
