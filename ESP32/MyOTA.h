// MyOTA.h

#ifndef _MYOTA_h
#define _MYOTA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Base.h"
#include "MySettings.h"

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

class MyOTAClass : public Base
{
 protected:


 public:
	void init();
	void loop();
	bool check();
};

extern MyOTAClass MyOTA;

#endif

