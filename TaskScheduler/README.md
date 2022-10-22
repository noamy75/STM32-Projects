# TaskScheduler
A bare-metal implementation of a round-robin scheduler. The tasks are managed in ready/blocked queues. The code features extensive stack manipulations and work with the processor’s core registers, mainly during context-switching, therefore inline assembly is used.  
The project was written without any libraries for an STM32F407 microcontroller.  
  
**Live Demonstration:** The leds are toggled in different frequencies - Green every 1s, Orange every 500ms, Blue every 250ms, Red every 125ms.
