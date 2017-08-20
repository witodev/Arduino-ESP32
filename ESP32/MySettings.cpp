// 
// 
// 

#include "MySettings.h"

void MySettingsClass::init()
{
	Enabled = check();

	if (!Enabled)
	{
		Serial.println(">>> Settings failed");
		return;
	}

}

void MySettingsClass::loop()
{
}

bool MySettingsClass::check()
{
	File configFile = SPIFFS.open("/config.json", "r");
	if (!configFile) {
		Serial.println("Failed to open config file");
		return false;
	}

	size_t size = configFile.size();
	if (size > 1024) {
		Serial.println("Config file size is too large");
		return false;
	}

	std::unique_ptr<char[]> buf(new char[size]);
	configFile.readBytes(buf.get(), size);

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(buf.get());

	if (!json.success()) {
		Serial.println("Failed to parse config file");
		return false;
	}

	const char* serverName = json["ssid"];
	const char* accessToken = json["password"];
	
	//// Real world application would store these values in some variables for
	//// later use.
	//configFile.seek(0, SeekSet);
	//String txt = configFile.readString();

	//Serial.println();
	//Serial.println("Raw file:");
	//Serial.println(txt);
	//Serial.println();

	Serial.print("Loaded serverName: ");
	Serial.println(serverName);
	Serial.print("Loaded accessToken: ");
	Serial.println(accessToken);
	
	return true;
}



MySettingsClass MySettings;

