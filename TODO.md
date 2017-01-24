- spi
- power_all_disable(): portable way of turning off on-board devices
- attiny serial output and input (SoftwareSerial uses pin-change interrupts)
  check out: https://github.com/frank26080115/Arduino-UsiSerial-Library/

Changes to [cherry-pick](https://coderwall.com/p/sgpksw/git-cherry-pick-from-another-repository):
- ~~Fix examples [24f4da3](../../pull/1/commits/24f4da3da3708afffd84b18a1d0eae57e9efe090), [22a0c5e](../../pull/1/commits/22a0c5e7e95045155747080efe3635d3282b1631), [a00cdc6](../../pull/1/commits/a00cdc6fc1ee24cae271a7311230b0f440f8d6db)~~
- ~~Break header dependencies on Arduino.h [8bc5132](../../pull/1/commits/8bc51324a0a8600c6d9ac1b4640dca0f62cff15f), [c9826eb](../../pull/1/commits/c9826ebaf46e34b78b040b64785bccfb3ba6918c)~~
- Discard first measurement after ADC powered-up (other architectures) [9c0134d](../../pull/1/commits/9c0134dbec1f41d2fa2df3544dd93fbef94d8436)
- Serial output for attiny84 (attiny85) [f76e69f](../../pull/1/commits/f76e69ffd1273028b8b831aa7603468d7f8a4fe5)
- Reset WDT when enabling [0499c7a](../../pull/1/commits/0499c7abadb38f652cfb558203f3cfce84af30fc)
- Sleep and wake functions [9e34dda](../../pull/1/commits/9e34ddab7566d718a68a2f7b30f1cbf3192ddc96), [f53e65](../../pull/1/commits/bf53e651a071eaeaada8dfcc20f4c5533d0a0473), [27a7f34](../../pull/1/commits/27a7f3430aff0aec37fd08fb0d87d0e1e8d0a27b), [42b7317](../../pull/1/commits/42b731722ea856d91c36a9fae940b68708cdcd32). (Note that some of these commits contain bug-fixes which should be merged separately.)
