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
    #include "timer.h"
    
    Watchdog timer(5, 1);
    External ext0(2), ext1(3);
    Devices devices;
    
    void setup(void)
    {
    	Serial.begin(115200);
    
    	devices.add(timer);
    	devices.add(ext0);
    	devices.add(ext1);
    	devices.begin();
    	 
    	ext0.enable(false);             // LED initially on
        timer.enable(false);            // timer off
    
    	pinMode(13, OUTPUT);
    }
    
    void loop(void)
    {
    	switch (devices.select()) {
    	case 1:
    	case 2:
    		digitalWrite(13, HIGH);
    		ext0.enable(false);
    		timer.enable(false);
    		ext1.enable(true);
    		break;
    	case 3:
    		digitalWrite(13, LOW);
    		ext0.enable(true);
    		timer.enable(true);
    		ext1.enable(false);
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
  - if the timer elapses when the LED is off, it is turned on again.
* Three devices: a 
[watchdog timer](http://evothings.com/watchdog-timers-how-to-reduce-power-usage-in-your-arduino-projects/) 
  with id 1, interrupting every 5s and two
  [external interrupts](http://gonium.net/md/2006/12/20/handling-external-interrupts-with-arduino/) 
  on ports 2 and 3 (with ids 2 and 3).
* A device group ("devices") which manages them.
* Selecting on the device group sends the CPU to sleep if no device is ready.
* When a device is ready, select returns its id.
* When an external interrupt has been processed, it is disabled until it makes sense to re-enable it; so for example when the LED is switched on, its interrupt is disabled until it has been switched off again. 
* Disabled interrupts don't wake the CPU and aren't "remembered".

Credits
-------
* Nick Gammon's excellent [interrupt page](http://gammon.com.au/interrupts)
