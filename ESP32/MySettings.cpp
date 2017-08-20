// 
// 
// 

#include "MySettings.h"

bool MySettingsClass::saveConfig()
{
	//StaticJsonBuffer<200> jsonBuffer;
	//JsonObject& json = jsonBuffer.createObject();
	//json["ssid"] = "api.example.com";
	//json["password"] = "128du9as8du12eoue8da98h123ueh9h98";

	//File configFile = SPIFFS.open("/config.json", "w");
	//if (!configFile) {
	//	Serial.println("Failed to open config file for writing");
	//	return false;
	//}

	//json.printTo(configFile);
	return true;
}

void MySettingsClass::init()
{
	if (!SPIFFS.begin()) {
		Serial.println("Failed to mount file system");
		return;
	}

	Enabled = check();

	//auto ssid = GetByHeader("ssid");
	//auto pass = GetByHeader("pass");
	//auto aaa = GetByHeader("aaa");
	//Serial.print("ssid: ");
	//Serial.println(ssid);
	//Serial.print("pass: ");
	//Serial.println(pass);
	//Serial.print("aaa: ");
	//Serial.println(aaa);

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
	
	return true;
}

String MySettingsClass::GetByHeader(String header)
{
	File configFile = SPIFFS.open("/config.json", "r");
	
	size_t size = configFile.size();
	if (size > 1024) {
		Serial.println("Config file size is too large");
		return false;
	}
	
	std::unique_ptr<char[]> buf(new char[size]);
	configFile.readBytes(buf.get(), size);

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(buf.get());

	return String(json[header]);
}



MySettingsClass MySettings;

