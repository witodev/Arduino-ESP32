// MyOLED.h

#ifndef _MYOLED_h
#define _MYOLED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <SSD1306.h> // alias for `#include "SSD1306Wire.h"`
#define DELTA 500

class MyOLEDClass
{
 private:
	 SSD1306* display;
	 float BigText;
	 float SmallText;

	 long lastWifi;

 public:
	 void MyOLED();
	void init();
	void loop();

	void ShowWifi();
	void HideWifi();
	void SwitchWifi();

	void ShowRefresh();
	void HideRefresh();
	void SwitchRefresh();

	void ShowCloud();
	void HideCloud();
	void SwitchCloud();

	void Print(float value);
	void print(float value);

	void Clear();
};

extern MyOLEDClass MyOLED;

#endif

