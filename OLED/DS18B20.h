/*
 * DS18B20.h
 *
 *  Created on: Sep 7, 2017
 *      Author: wito
 */

#ifndef MAIN_DS18B20_H_
#define MAIN_DS18B20_H_

#define Tconv_12bit 750
//Time to Strong Pullup On
#define tSPON 10
//Time Slot
#define tSLOT_min 60
#define tSLOT_max 120
//Recovery Time
#define tREC 1
//Write 0 Low Time
#define tLOW0_min 60
#define tLOW0_max 120
//Write 1 Low Time
#define tLOW1_min 1
#define tLOW1_max 15
//Read Data Valid
#define tRDV 15
//Reset Time High
#define tRSTH 480
//Reset Time Low
#define tRSTL 480
//Presence-Detect High
#define tPDHIGH_min 15
#define tPDHIGH_max 60
//Presence-Detect Low
#define tPDLOW_min 60
#define tPDLOW_max 240
#define DummyVal -127.0f

#include <sdkconfig.h>
#include <esp_system.h>
#include <driver/gpio.h>
#include <rom/ets_sys.h>
#include <esp_log.h>
#include <string>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

typedef uint8_t Address[8];

class DS18B20
{
private:
	uint8_t LastDeviceFlag;
	uint8_t LastDiscrepancy;
	uint8_t LastFamilyDiscrepancy;
	unsigned char ROM_NO[8];
	gpio_num_t myPin;

	void init(int GPIO);
	unsigned char reset(void);
	void write_bit(char bit);
	void write(uint8_t data);
	unsigned char read_bit(void);
	unsigned char read(void);
	uint8_t search(uint8_t *newAddr, bool search_mode);
	void reset_search();
	void select(const uint8_t rom[8]);

	uint8_t devCount;
	Address addresses[10];

public:
	DS18B20(gpio_num_t wirePin);
	DS18B20(int wirePin);
	virtual ~DS18B20();

	uint8_t FindDevices();
	uint8_t Count();
	std::string Dev(int index = 0);
	float TempC(int index = 0);
};

#endif /* MAIN_DS18B20_H_ */
