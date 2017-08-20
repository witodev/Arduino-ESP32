// Example: storing JSON configuration file in flash file system
//
// Uses ArduinoJson library by Benoit Blanchon.
// https://github.com/bblanchon/ArduinoJson
//
// Created Aug 10, 2015 by Ivan Grokhotkov.
//
// This example code is in the public domain.

#include <ArduinoJson.h>
#include <SPIFFS.h>

const char* ssid = "...";
const char* pass = "...";

char* Convert(const char* source)
{
	auto len = strlen(source);
	auto buff = new char[len];
	strcpy(buff, source);
	return buff;
}

bool loadConfig() {
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

	// Allocate a buffer to store contents of the file.
	std::unique_ptr<char[]> buf(new char[size]);

	// We don't use String here because ArduinoJson library requires the input
	// buffer to be mutable. If you don't use ArduinoJson, you may as well
	// use configFile.readString instead.
	configFile.readBytes(buf.get(), size);

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(buf.get());

	if (!json.success()) {
		Serial.println("Failed to parse config file");
		return false;
	}

	const char* serverName = json["ssid"];
	const char* accessToken = json["pass"];

	char buff[100];
	strcpy(buff, (const char*)json["ssid"]);
	ssid = buff;
	ssid = Convert(json["ssid"]);

	pass = Convert(json["pass"]);

	// Real world application would store these values in some variables for
	// later use.
	configFile.seek(0, SeekSet);
	String txt = configFile.readString();

	Serial.println();
	Serial.println("Raw file:");
	Serial.println(txt);
	Serial.println();

	Serial.print("Loaded serverName: ");
	Serial.println(serverName);
	Serial.print("Loaded accessToken: ");
	Serial.println(accessToken);
	return true;
}

bool saveConfig() {
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& json = jsonBuffer.createObject();
	json["ssid"] = "FunBox2-F197";
	json["pass"] = "E13FE673E25A52323F4EC1444A";

	File configFile = SPIFFS.open("/config.json", "w");
	if (!configFile) {
		Serial.println("Failed to open config file for writing");
		return false;
	}

	json.printTo(configFile);
	return true;
}

void setup() {
	Serial.begin(115200);
	Serial.println("");
	delay(1000);
	Serial.println("Mounting FS...");

	if (!SPIFFS.begin()) {
		Serial.println("Failed to mount file system");
		return;
	}
	else
	{
		Serial.println("FS OK");
	}

	if (!saveConfig()) {
		Serial.println("Failed to save config");
	}
	else {
		Serial.println("Config saved");
	}

	if (!loadConfig()) {
		Serial.println("Failed to load config");
	}
	else {
		Serial.println("Config loaded");
		Serial.print("SSID: ");
		Serial.println(ssid);
		Serial.print("PASS: ");
		Serial.println(pass);
	}
}

void loop() {
}