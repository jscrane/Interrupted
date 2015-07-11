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
