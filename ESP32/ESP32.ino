#include "MyOLED.h"

void setup() {
	Serial.begin(115200);
	Serial.println();
	Serial.println();
	
	MyOLED.init();
	randomSeed(millis());
}

long last = 0;
void BlinkWifi()
{
	if (millis() - last > 500)
	{
		MyOLED.SwitchWifi();
		MyOLED.SwitchCloud();
		MyOLED.SwitchRefresh();
		last = millis();
	}
}

void loop() {
	MyOLED.loop();
	if (millis() - last > 1300)
	{
		MyOLED.Print((random(6000) - 3000) / 100.0);
		last = millis();
	}
	yield();
}