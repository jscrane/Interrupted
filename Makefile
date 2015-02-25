PLATFORM := attiny
HARDWARE_FAMILY := $(HOME)/sketchbook/hardware/attiny/avr
CORE := $(HARDWARE_FAMILY)/cores/tiny
BOARD := t84db
SKETCH := tinytimer.ino
#SKETCH := tinyadc.ino
#SKETCH := tinytest.ino
include arduino15.mk
