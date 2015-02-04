#PLATFORM := attiny
#HARDWARE_FAMILY := $(HOME)/sketchbook/hardware/attiny/avr
#CORE := $(HARDWARE_FAMILY)/cores/tiny
#BOARD := t84db
#SKETCH := tinytest.ino
#include arduino15.mk
BOARD := uno
SKETCH := megatest.ino
include arduino10.mk
