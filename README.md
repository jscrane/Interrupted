Interrupt Driven
================

A new approach to building low-power apps on Arduino:
* The main state of the processor is sleeping in a low-power mode.
* Devices wake the processor using interrupts, it services them before returning to sleep.
* State-machines are easily implemented: devices may be temporarily disabled when not needed by the current state.

Example
-------

    #include "device.h"
    #include "external.h"
    #include "pinchange.h"
    #include "timer.h"
    
    Watchdog timer(5, 1);
    External int0(2), int1(3);
    PinChangeGroup group(D8_13);
    PinChange led(group, 13);
    Devices devices;
    
    void setup(void)
    {
    	Serial.begin(115200);
    
    	devices.add(timer);
    	devices.add(int0);
    	devices.add(int1);
        devices.add(led);
    	devices.begin();
    	 
    	pinMode(13, OUTPUT);
    }
    
    void loop(void)
    {
    	switch (devices.select()) {
    	case 2:
    		digitalWrite(13, HIGH);
    		break;
    	case 1:
    	case 3:
    		digitalWrite(13, LOW);
    		break;
    	case 13:
            timer.enable(led.is_on());
            int0.enable(!led.is_on());
            int1.enable(led.is_on());
    		break;
    	default:
    		Serial.println("???");
    		break;
    	}
    
    	Serial.println("awake");
    	delay(100);
    }

Comments on this code:
* This sketch manages a configuration with pins #2 and #3 connected to GND via a pushbutton:
  - when the button on pin #2 is pressed the LED on pin #13 is lit,
  - when that on pin #3 is pressed and released, the LED is turned off,
  - if the timer elapses when the LED is on, it is turned off again.
* Three devices: a 
[watchdog timer](http://evothings.com/watchdog-timers-how-to-reduce-power-usage-in-your-arduino-projects/) 
  with id 1, interrupting every 5s and two
  [external interrupts](http://gonium.net/md/2006/12/20/handling-external-interrupts-with-arduino/) 
  on ports 2 and 3 (with ids 2 and 3).
* A device group ("devices") which manages them.
* Selecting on the device group sends the CPU to sleep if no device is ready.
* When a device is ready, select returns its id.
* Change of state of the LED pin triggers a state-machine change: if the LED has come on, the timer and external interrupt #1 are enabled, if the LED has been switched off, external interrupt #0 is enabled.
* Disabled interrupts don't wake the CPU and aren't "remembered".

Credits
-------
* Nick Gammon's excellent [interrupt page](http://gammon.com.au/interrupts).
