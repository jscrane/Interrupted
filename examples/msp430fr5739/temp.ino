#include <Interrupted.h>

/*
 * Reads the NTC on A1
 * See: https://github.com/energia/Energia/tree/master/examples/06.Sensors/Thermometer_FRAUNCHPAD
 */
#define SEROUT	99

SerialOut<> output(SEROUT, TERMINAL_SPEED);
Analog adc(A1);
Devices devices(adc, output);

#define vRef 3000
#define rDiv 470000000

void setup(void)
{
	devices.begin();

	pinMode(A1, INPUT);
	pinMode(NTC_ENABLE, OUTPUT);
	digitalWrite(NTC_ENABLE, HIGH);
	adc.enable();
}

static int32_t table[21][2] = {
  {-25, 1344300},
  {-20,  998530},
  {-15,  748670},
  {-10,  566360},
  { -5,  432120},
  {  0,  332400},
  {  5,  257690},
  { 10,  201270},
  { 15,  158340},
  { 20,  125420},
  { 25,  100000},
  { 30,   80239},
  { 35,   64776},
  { 40,   52598},
  { 45,   42950},
  { 50,   35262},
  { 55,   29100},
  { 60,   24136},
  { 65,   20114},
  { 70,   16841},
  { 75,   14164}
};

void loop(void)
{
	switch(devices.select()) {
	case SEROUT:
		adc.enable();
		return;
	case A1:
		break;
	default:
		return;
	}

	int64_t t = adc.read();
	t = (vRef * t) / 1024;
	t = (rDiv * t) / (vRef - t) / 1000;

	int32_t c = 9999;
	for (int i=1; i < 21; i++)
    		if (table[i][1] < t) {
      			c = map(t, table[i-1][1], table[i][1], 100*table[i-1][0], 100*table[i][0]);
      			c /= 10;
      			break;
    		}
	output.println(c);
}
