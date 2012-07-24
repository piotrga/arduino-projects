#include "WDT.h"
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

void (*interrupt_handler)() = 0;

void set_WDTCSR(unsigned ps){
  // The sequence for clearing WDE and changing configuration is as follows:
  //	1. In the same operation, write a logic one to the Watchdog change enable bit (WDCE) and WDE. A logic one must be written to WDE regardless of the previous value of the WDE bit.
  //	2. Within the next four clock cycles, write the WDE and Watchdog prescaler bits (WDP) as desired, but with the WDCE bit cleared. This must be done in one operation.
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  WDTCSR = ps;
}

void WDT::enable_with_interrupt_handler(unsigned delay, void (*handler)()){
  wdt_disable();
  cli(); // disable interrupts
  interrupt_handler = handler; // handler is called when wdt interrupt ocurrs
  wdt_reset(); // reset wdt counter
  set_WDTCSR(delay | 1<< WDIE); // delay + interrupt flag
  sei(); // re-enable interrupts
}

void WDT::disable(){
  wdt_disable();
  interrupt_handler = 0;
}

void disable_wdt_MCU_reset(){ 
    // If the Watchdog is accidentally enabled, for example by a runaway pointer or brown-out condition,
    // the device will be reset and the Watchdog Timer will stay enabled. If the code is not set up to handle
    // the Watchdog, this might lead to an eternal loop of time-out resets. To avoid this situation, 
    // the application software should always clear the Watchdog System Reset Flag (WDRF) and the WDE control
    // bit in the initialization routine, even if the Watchdog is not in use.
    MCUSR &= ~(1<<WDRF); 
}



ISR(WDT_vect) { // The ISR macro installs an interrupt handler for wdt. It delegates to interrupt_handler function.
  if (interrupt_handler){
    interrupt_handler();
  }
}

WDT wdt;
void (*after_wake_up_handler)() = 0;

void after_wake_up(){
  ADCSRA |= 1<<ADEN;  
  if (after_wake_up_handler) {
    after_wake_up_handler();
  }
}

EnergySaver::EnergySaver(unsigned sleepCycles){
  wdt.enable_with_interrupt_handler(sleepCycles, after_wake_up);
}

void EnergySaver::on_wake_up_call(void (*handler)()){
  cli();
  after_wake_up_handler = handler;
  sei();
}

void EnergySaver::sleep_and_save_power(){
  ADCSRA &= ~(1<<ADEN); // disable ADC (analog-digital converter)

  MCUCR |= 1<< BODS | 1<< BODSE; // disable BOD
  MCUCR = MCUCR & ~(1<<BODSE) | 1<<BODS; 
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // conserves most power
  sleep_mode();  
}


