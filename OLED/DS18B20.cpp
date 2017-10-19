/*
 * DS18B20.cpp
 *
 *  Created on: Sep 7, 2017
 *      Author: wito
 */

#include "DS18B20.h"

DS18B20::DS18B20(gpio_num_t wirePin)
{
	init((int) wirePin);
}

DS18B20::DS18B20(int wirePin)
{
	init(wirePin);
}

void DS18B20::init(int GPIO)
{
	//ESP_LOGD("DS18B20", "init");

	myPin = (gpio_num_t) GPIO;
	gpio_pad_select_gpio(myPin);

	LastDeviceFlag = 0;
	LastDiscrepancy = 0;
	LastFamilyDiscrepancy = 0;
}

unsigned char DS18B20::reset(void)
{
	unsigned char PRESENCE = 0;

//	RESET PULSE FROM HOST
	gpio_set_direction(myPin, GPIO_MODE_OUTPUT);
	gpio_set_level(myPin, 0);
	ets_delay_us(tRSTL);
	gpio_set_level(myPin, 1);

//	PRESENCE DETECT
	gpio_set_direction(myPin, GPIO_MODE_INPUT);
	ets_delay_us(tPDHIGH_max);
	PRESENCE = 1 - gpio_get_level(myPin);

	ets_delay_us(tRSTH - tPDLOW_max);

//	printf("> return PRESENCE: %i\n", PRESENCE);
	return PRESENCE;
}

void DS18B20::write_bit(char bit)
{
	gpio_set_direction(myPin, GPIO_MODE_OUTPUT);
	gpio_set_level(myPin, 0);
	ets_delay_us(1);
	if (bit == 1)
		gpio_set_level(myPin, 1);
	ets_delay_us(tLOW0_min);
	gpio_set_level(myPin, 1);
}

void DS18B20::write(uint8_t data)
{
	unsigned char i;
	unsigned char x;
	for (i = 0; i < 8; i++)
	{
		x = data >> i;
		x &= 0x01;
		write_bit(x);
	}
	ets_delay_us(100);
}

unsigned char DS18B20::read_bit(void)
{
	unsigned char PRESENCE = 0;
	gpio_set_direction(myPin, GPIO_MODE_OUTPUT);
	gpio_set_level(myPin, 0);
	ets_delay_us(1);
	gpio_set_level(myPin, 1);
	ets_delay_us(tPDHIGH_min);
	gpio_set_direction(myPin, GPIO_MODE_INPUT);
	if (gpio_get_level(myPin) == 1)
		PRESENCE = 1;
	else
		PRESENCE = 0;
	ets_delay_us(tSLOT_min - tPDHIGH_min);
	return (PRESENCE);
}

unsigned char DS18B20::read(void)
{
	unsigned char i;
	unsigned char data = 0;
	for (i = 0; i < 8; i++)
	{
		if (read_bit())
			data |= 0x01 << i;
		ets_delay_us(tPDHIGH_min);
	}
	return (data);
}

uint8_t DS18B20::search(uint8_t* newAddr, bool search_mode)
{
	uint8_t id_bit_number;
	uint8_t last_zero, rom_byte_number, search_result;
	uint8_t id_bit, cmp_id_bit;

	unsigned char rom_byte_mask, search_direction;

	// initialize for search
	id_bit_number = 1;
	last_zero = 0;
	rom_byte_number = 0;
	rom_byte_mask = 1;
	search_result = 0;

	// if the last call was not the last one
	if (!LastDeviceFlag)
	{
		// 1-Wire reset
		if (!reset())
		{
			//			printf("// reset the search\n");
			LastDiscrepancy = 0;
			LastDeviceFlag = false;
			LastFamilyDiscrepancy = 0;
			return false;
		}

		// issue the search command
		if (search_mode == true)
		{
			write(0xF0);
			//			printf("// NORMAL SEARCH\n");
		}
		else
		{
			write(0xEC);
			//			printf("// CONDITIONAL SEARCH\n");
		}

		// loop to do the search
		do
		{
			// read a bit and its complement
			id_bit = read_bit();
			cmp_id_bit = read_bit();

			// check for no devices on 1-wire
			if ((id_bit == 1) && (cmp_id_bit == 1))
			{
				//				printf("// check for no devices on 1-wire\n");
				break;
			}
			else
			{
				// all devices coupled have 0 or 1
				if (id_bit != cmp_id_bit)
					search_direction = id_bit; // bit write value for search
				else
				{
					// if this discrepancy if before the Last Discrepancy
					// on a previous next then pick the same as last time
					if (id_bit_number < LastDiscrepancy)
						search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
					else
						// if equal to last pick 1, if not then pick 0
						search_direction = (id_bit_number == LastDiscrepancy);

					// if 0 was picked then record its position in LastZero
					if (search_direction == 0)
					{
						last_zero = id_bit_number;

						// check for Last discrepancy in family
						if (last_zero < 9)
							LastFamilyDiscrepancy = last_zero;
					}
				}

				// set or clear the bit in the ROM byte rom_byte_number
				// with mask rom_byte_mask
				if (search_direction == 1)
					ROM_NO[rom_byte_number] |= rom_byte_mask;
				else
					ROM_NO[rom_byte_number] &= ~rom_byte_mask;

				// serial number search direction write bit
				write_bit(search_direction);

				// increment the byte counter id_bit_number
				// and shift the mask rom_byte_mask
				id_bit_number++;
				rom_byte_mask <<= 1;

				// if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
				if (rom_byte_mask == 0)
				{
					rom_byte_number++;
					rom_byte_mask = 1;
				}
			}
		} while (rom_byte_number < 8); // loop until through all ROM bytes 0-7

		// if the search was successful then
		if (!(id_bit_number < 65))
		{
			// search successful so set LastDiscrepancy,LastDeviceFlag,search_result
			LastDiscrepancy = last_zero;

			// check for last device
			if (LastDiscrepancy == 0)
				LastDeviceFlag = true;

			search_result = true;
		}
	}

	// if no device found then reset counters so next 'search' will be like a first
	if (!search_result || !ROM_NO[0])
	{
		LastDiscrepancy = 0;
		LastDeviceFlag = false;
		LastFamilyDiscrepancy = 0;
		search_result = false;
	}
	else
	{
		for (int i = 0; i < 8; i++)
			newAddr[i] = ROM_NO[i];
	}
	return search_result;
}

void DS18B20::reset_search()
{
	// reset the search state
	LastDiscrepancy = 0;
	LastDeviceFlag = false;
	LastFamilyDiscrepancy = 0;
	for (int i = 7; i >= 0; i--)
	{
		ROM_NO[i] = 0;
	}
}

void DS18B20::select(const uint8_t rom[8])
{
	uint8_t i;

	write(0x55); // Choose ROM

	for (i = 0; i < 8; i++)
		write(rom[i]);
}

DS18B20::~DS18B20()
{
	// TODO Auto-generated destructor stub
}

uint8_t DS18B20::FindDevices()
{
	//ESP_LOGD("DS18B20", "FindDevices");
	devCount = 0;
	uint8_t addr[8];
	reset_search();
	while (search(addr, true))
	{
		for (int i = 0; i < 8; i++)
			addresses[devCount][i] = addr[i];
		devCount++;
	}
	reset_search();
	return devCount;
}

uint8_t DS18B20::Count()
{
	//ESP_LOGD("DS18B20", "Count");
	return devCount;
}

std::string DS18B20::Dev(int index)
{
	//ESP_LOGD("DS18B20", "Dev");
	if (index > devCount)
		index = 0;

	uint8_t addr[8];
	for (int i = 0; i < 8; i++)
		addr[i] = addresses[index][i];

	char buffer[17];
	buffer[16] = 0;
	for (int j = 0; j < 8; j++)
		sprintf(&buffer[2 * j], "%02X", addr[j]);

	return std::string(buffer);
}

float DS18B20::TempC(int index)
{
	//ESP_LOGD("DS18B20", "TempC");
	if (index > devCount)
		index = 0;

	uint8_t addr[8];
	for (int i = 0; i < 8; i++)
		addr[i] = addresses[index][i];

	reset();
	select(addr);
	write(0x44); // start conversion

	vTaskDelay(Tconv_12bit / portTICK_RATE_MS);

	uint8_t check = reset();
	select(addr);
	write(0xBE); // Read Scratchpad

	float temp1 = read();
	float temp2 = read();
	check = reset();
	float temp = 0;
	temp = (float) (temp1 + (temp2 * 256)) / 16;
	if (check)
		return temp;
	else
		return DummyVal;
}
