Interrupt Driven
================

* A new approach to building low-power apps on Arduino.
* The main state of the processor is a low-power mode.
* Devices wake the processor using interrupts, it services them and
  returns to sleep.

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
    
    	pinMode(13, OUTPUT);
    }
    
    void loop(void)
    {
    	switch (devices.select()) {
    	case 1:
    		Serial.println("tick");
    		break;
    	case 2:
    		digitalWrite(13, HIGH);
    		ext0.enable(false);     // only allow ext1 to interrupt now
    		ext1.enable(true);
    		break;
    	case 3:
    		digitalWrite(13, LOW);
    		ext0.enable(true);     // only allow ext0 to interrupt now
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
* Three devices: a 
[watchdog timer](http://evothings.com/watchdog-timers-how-to-reduce-power-usage-in-your-arduino-projects/) 
  with id 1, interrupting every 5s and two
  [external interrupts](http://gonium.net/md/2006/12/20/handling-external-interrupts-with-arduino/) 
  on ports 2 and 3 (and ids 2 and 3).
* A device group ("devices") which manages them.
* Selecting on the device group sends the CPU to sleep if no device is ready.
* When a device is ready, it returns its id.
* Each external interrupt line is connected to ground by a push-button; when
  the one attached to pin #2 is pressed, the LED on pin #13 is lit, when
  that on pin #3 is pressed, the LED is switched off.
* When an external interrupt has been processed, it is disabled until it makes sense to re-enable it; so for example when the LED is switched on, its interrupt is disabled until it has been switched off again. Disabled interrupts don't wake the CPU and aren't "remembered".

Credits
-------
* Nick Gammon's excellent [interrupt page](http://gammon.com.au/interrupts)
