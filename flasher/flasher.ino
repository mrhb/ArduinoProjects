/****************************************************************************************************************************
  Argument_None.ino
  For Arduino and Adadruit AVR 328(P) and 32u4 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/TimerInterrupt
  Licensed under MIT license

  Now we can use these new 16 ISR-based timers, while consuming only 1 hardware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Notes:
  Special design is necessary to share data between interrupt code and the rest of your program.
  Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume
  variable can not spontaneously change. Because your function may change variables while your program is using them,
  the compiler needs this hint. But volatile alone is often not enough.
  When accessing shared variables, usually interrupts must be disabled. Even with volatile,
  if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly.
  If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled
  or the entire sequence of your code which accesses the data.
 *****************************************************************************************************************************/

// These define's must be placed at the beginning before #include "TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#define USE_TIMER_1     true
 
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "TimerInterrupt.h"

void TimerHandler1(void)
{
  static bool toggle1 = false;

  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(LED_BUILTIN, toggle1);
  toggle1 = !toggle1;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  ITimer1.init();
  updateState(0);
}

void loop()
{
  delay(5000);
  updateState(1);
  delay(5000);
  updateState(2);
  delay(5000);
  updateState(3);
  delay(5000);
  updateState(0);
}
void updateState(uint8_t state)
{
  switch (state) {
  case 0:
    ITimer1.attachInterruptInterval(50, TimerHandler1);
    break;
  case 1:
    ITimer1.attachInterruptInterval(100, TimerHandler1);
    break;
  case 2:
    ITimer1.attachInterruptInterval(200, TimerHandler1);
    break;
  case 3:
    ITimer1.attachInterruptInterval(300, TimerHandler1);
    break;
  }
}