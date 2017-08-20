// MySettings.h

#ifndef _MYSETTINGS_h
#define _MYSETTINGS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Base.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>

class MySettingsClass : public Base
{
 protected:
	 bool saveConfig();

 public:
	void init();
	void loop();
	bool check();

	String GetByHeader(String header);
};

extern MySettingsClass MySettings;

#endif

