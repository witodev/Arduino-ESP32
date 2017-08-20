// 
// 
// 

#include "MyOLED.h"
#include "small_images.h"

void MyOLEDClass::MyOLED()
{
	lastWifi = 0;
}

void MyOLEDClass::init()
{
	display = new SSD1306(0x3c, 22, 21);
	// Initialising the UI will init the display too.
	display->init();

	display->flipScreenVertically();
	display->setFont(ArialMT_Plain_10);
	//display->clear();
	//display->display();
	Clear();
}

void MyOLEDClass::loop()
{
	if (millis() - lastWifi > DELTA)
	{
		SwitchWifi();
		lastWifi = millis();
	}

	Print(BigText);
	print(SmallText);
}

void MyOLEDClass::ShowWifi()
{
	display->setColor(WHITE);
	display->drawXbm(wifi_left, wifi_top, wifi_width, wifi_height, wifi_bits);
	display->display();
}

void MyOLEDClass::HideWifi()
{
	display->setColor(BLACK);
	display->fillRect(wifi_left, wifi_top, wifi_width, wifi_height);
	display->display();
	display->setColor(WHITE);
}

void MyOLEDClass::SwitchWifi()
{
	if (show_wifi)
		ShowWifi();
	else
		HideWifi();
		
	show_wifi = !show_wifi;
}

void MyOLEDClass::ShowRefresh()
{
	display->setColor(WHITE);
	display->drawXbm(refresh_left, refresh_top, refresh_width, refresh_height, refresh_bits);
	display->display();
}

void MyOLEDClass::HideRefresh()
{
	display->setColor(BLACK);
	display->fillRect(refresh_left, refresh_top, refresh_width, refresh_height);
	display->display();
	display->setColor(WHITE);
}

void MyOLEDClass::SwitchRefresh()
{
	if (show_refresh)
		ShowRefresh();
	else
		HideRefresh();

	show_refresh = !show_refresh;
}

void MyOLEDClass::ShowCloud()
{
	display->setColor(WHITE);
	display->drawXbm(cloud_left, cloud_top, cloud_width, cloud_height, cloud_bits);
	display->display();
}

void MyOLEDClass::HideCloud()
{
	display->setColor(BLACK);
	display->fillRect(cloud_left, cloud_top, cloud_width, cloud_height);
	display->display();
	display->setColor(WHITE);
}

void MyOLEDClass::SwitchCloud()
{
	if (show_clound)
		ShowCloud();
	else
		HideCloud();

	show_clound = !show_clound;
}

void MyOLEDClass::Print(float value)
{
	if (value != BigText)
	{
		display->setColor(BLACK);
		display->fillRect(0, 16, 128, 35);
		display->display();
	}
	BigText = value;
	display->setColor(WHITE);

	display->setFont(SansSerif_plain_35);
	display->setTextAlignment(TEXT_ALIGN_CENTER);
	String msg(value);
	display->drawString(64, 17, msg);
	display->display();
}

void MyOLEDClass::print(float value)
{
	if (value != SmallText)
	{
		display->setColor(BLACK);
		display->fillRect(0, 52, 128, 64);
		display->display();
	}
	SmallText = value;
	display->setColor(WHITE);

	display->setFont(ArialMT_Plain_10);
	display->setTextAlignment(TEXT_ALIGN_CENTER);
	String msg(value);
	display->drawString(64, 52, msg);
	display->display();
}

void MyOLEDClass::Clear()
{
	display->clear();
	display->display();
}

MyOLEDClass MyOLED;

