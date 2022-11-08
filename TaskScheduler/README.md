# TaskScheduler
A bare-metal implementation of a round-robin scheduler. The tasks are managed in ready/blocked queues. The code features extensive stack manipulations and work with the processor’s core registers, mainly during context-switching, therefore inline assembly is used.  
The project was written without any libraries for an STM32F407 microcontroller.  
  
**Live Demonstration:** The leds are toggled in different frequencies - Green every 1s, Orange every 500ms, Blue every 250ms, Red every 125ms.  
  
https://user-images.githubusercontent.com/96314781/197357281-cf3d95a2-b070-4ac1-a71d-2734f8990f1d.mp4

**Memory organization:**

![image](https://user-images.githubusercontent.com/96314781/200632540-3c752f1e-c418-4b1a-871d-9e6d0291c375.png)

**Ready & Blocked Queues:**

