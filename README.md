Interrupted
===========

A new approach to building low-power, interrupt-driven, apps on Arduino:
* The processor mostly sleeps in a low-power mode, as low as possible given
 the currently-enabled devices.
* Devices wake the processor using interrupts, it services them before returning to sleep.
* State-machines are easily implemented: devices may be temporarily disabled 
  when not needed in the current state.
  
Note
----
* The master branch of this library is mostly compatible with the Arduino IDE;
however it currently requires 
[uC-Makefile](https://github.com/jscrane/uC-Makefile) to build the examples.
* The ATtiny parts of this require Spence Konde's excellent [ATTinyCore](https://github.com/SpenceKonde/ATTinyCore). Only t84 and t85 are supported for now.
* This has been tested with Arduino 1.8.9 and Energia 1.8.7E21.

See [blog articles](http://programmablehardware.blogspot.ie/search/label/interrupted).

Example
-------

    #include <Interrupted.h>

    #define LED     13
    #define SER_OUT 99
    #define EXT0    2
    #define EXT1    3
    #define TIMER   1

    SerialOut output(SER_OUT, 115200);
    Watchdog timer(TIMER, 4);
    External int0(EXT0), int1(EXT1, RISING);
    Pin led(LED);
    Devices devices(timer, int0, int1, led, output);
    
    void setup(void)
    {
      devices.begin();
    	 
      pinMode(LED, OUTPUT);
      digitalWrite(LED, HIGH);
    }
    
    void loop(void)
    {
      switch (devices.select()) {
      case EXT0:
        digitalWrite(LED, HIGH);
        break;
      case TIMER:
      case EXT1:
        digitalWrite(LED, LOW);
        break;
      case LED:
        int0.enable(!led.is_on());
        int1.enable(led.is_on());
        timer.enable(led.is_on());
        break;
      case SER_OUT:
        output.enable(false);
        return;
      }
      output.enable();
      output.write("awake!\r\n");
    }

Remarks:
* This sketch manages a configuration with pins #2 and #3 connected to GND via 
pushbuttons:
  - when the button on pin #2 is pressed the LED on pin #13 is lit,
  - when that on pin #3 is pressed and released, the LED is turned off,
  - if the timer elapses when the LED is on, it is turned off again.
* Five devices: 
  - a [watchdog timer](http://evothings.com/watchdog-timers-how-to-reduce-power-usage-in-your-arduino-projects/) with id 1, interrupting every 4s,
  - two [external interrupts](http://gonium.net/md/2006/12/20/handling-external-interrupts-with-arduino/) on ports 2 and 3,
  - a pin-change interrupt watching the LED on pin #13,
  - hardware serial output device running at 115200 baud.
* A device group ("devices") which manages them.
* Selecting on the device group sends the CPU to sleep if no device is ready.
* When a device is ready, select returns its id.
* Change of state of the LED pin triggers a state-machine change: 
  - if the LED has come on, the timer and external interrupt #1 are enabled, 
  - if the LED has been switched off, external interrupt #0 is enabled.
* Disabled interrupts don't wake the CPU and aren't "remembered".
* The serial port acts like any other device except that the processor 
must sleep in an IDLE mode when it is in use.
* The processor enters the deepest sleep allowed by the currently-enabled 
devices. When output is in progress, this is SLEEP_MODE_IDLE, when not,
SLEEP_MODE_POWER_DOWN.

Credits
-------
* Nick Gammon's excellent [interrupt](http://gammon.com.au/interrupts)
  and [power-saving](http://www.gammon.com.au/forum/?id=11497) pages.
* The [Conic and Regis](http://www-dse.doc.ic.ac.uk/cgi-bin/moin.cgi/Research) 
projects at [Imperial College London](http://wp.doc.ic.ac.uk/dse/).
* The [select](http://unixhelp.ed.ac.uk/CGI/man-cgi?select+2) system call.
* [icaven](https://github.com/icaven) for bugfixes and improvements.
