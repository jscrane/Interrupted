archs := atmega328p attiny84 attiny85 msp430fr5739 msp430g2553
examples := $(foreach a,$(archs),$(wildcard $a/*.ino))

all: $(examples)

$(examples):
	$(MAKE) -C $(dir $@) SKETCH=$(notdir $@) all

clean: $(archs)

$(archs):
	$(MAKE) -C $@ clean

.PHONY: all $(examples) $(archs) clean
