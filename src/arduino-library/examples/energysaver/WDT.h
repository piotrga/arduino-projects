#ifndef WDT_h
#define WDT_h

class WDT{
public:
  void enable_with_interrupt_handler(unsigned delay,  void (*handler)());
  void disable();
};

void disable_wdt_MCU_reset();

#define WDT_512K (1<<WDP3)
#define WDT_1024K (1<<WDP3 | 1<<WDP0)

class EnergySaver{
public:
  EnergySaver(unsigned sleepCycles);
  void sleep_and_save_power();
  void on_wake_up_call(void (*handler)());
};


#endif
