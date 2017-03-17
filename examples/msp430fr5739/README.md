Serial Port on Linux
====================
If you don't feel like recompiling the kernel, this is a quick workaround
for viewing serial output on Linux:
 - program the launchpad
 - unplug it
 - press and hold the reset button
 - plug launchpad back in
 - wait for permissions on /dev/ttyACM0 to settle
 - run "minicom -b 9600 -D /dev/ttyACM0"
 - release reset button

However, the reset button on the Fraunchpad is a bit too small to do this
comfortably, so there's nothing for it but to recompile the kernel module
(at least on Linux 3.11): See [Fix a Linux Kernel bug...](https://orenkishon.wordpress.com/2014/06/29/msp430-ez430-rf2500-linux-support-guide/)

Update: this is fixed under kernel 3.16.
