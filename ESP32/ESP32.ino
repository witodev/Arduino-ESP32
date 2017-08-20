#include "MySettings.h"
#include "MyOTA.h"
#include "MyOLED.h"

long last = 0;

void setup() {
	Serial.begin(115200);
	Serial.println("==> START <==");
	Serial.println();

	MySettings.init();
	MyOTA.init();
}

void loop() {
	MyOTA.loop();
}