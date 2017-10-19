/*
 * 1. Open up the project properties
 * 2. Visit C/C++ General > Preprocessor Include Paths, Macros, etc
 * 3. Select the Providers tab
 * 4. Check the box for "CDT GCC Built-in Compiler Settings"
 * 5. Set the compiler spec command to "xtensa-esp32-elf-gcc ${FLAGS} -E -P -v -dD "${INPUTS}""
 * 6. Rebuild the index
 */

#include <stdio.h>
#include <esp_log.h>
#include <string>
#include "sdkconfig.h"

#include "SSD1306.h"
#include "DS18B20.h"

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <string.h>
#include <u8g2.h>
#include <U8G2.h>

extern "C"
{
#include "u8g2_esp32_hal.h"
}

const char tag[] = "myoled";
// SDA - GPIO21
#define PIN_SDA ((gpio_num_t)(19))

// SCL - GPIO22
#define PIN_SCL ((gpio_num_t)(23))

static const char *TAG = "ssd1306";

#define delay(ms) vTaskDelay((ms)/portTICK_PERIOD_MS);

SSD1306 oled;

void task_test_SSD1306i2c(void *ignore) {
	u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
	u8g2_esp32_hal.sda   = PIN_SDA;
	u8g2_esp32_hal.scl  = PIN_SCL;
	u8g2_esp32_hal_init(u8g2_esp32_hal);


	u8g2_t u8g2; // a structure which will contain all the data for one display
//	u8g2_Setup_ssd1306_128x64_noname_f(
	u8g2_Setup_ssd1306_128x64_vcomh0_f(
		&u8g2,
		U8G2_R2,
		//u8x8_byte_sw_i2c,
		u8g2_esp32_msg_i2c_cb,
		u8g2_esp32_msg_i2c_and_delay_cb);  // init u8g2 structure
	u8x8_SetI2CAddress(&u8g2.u8x8,0x78);

	ESP_LOGI(TAG, "u8g2_InitDisplay");
	u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,

	ESP_LOGI(TAG, "u8g2_SetPowerSave");
	u8g2_SetPowerSave(&u8g2, 0); // wake up display
	ESP_LOGI(TAG, "u8g2_ClearBuffer");
	u8g2_ClearBuffer(&u8g2);
	ESP_LOGI(TAG, "u8g2_DrawBox");
	u8g2_DrawBox(&u8g2, 0, 26, 80,6);
	u8g2_DrawFrame(&u8g2, 0,26,100,6);

	ESP_LOGI(TAG, "u8g2_SetFont");
    u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
	ESP_LOGI(TAG, "u8g2_DrawStr");
    u8g2_DrawStr(&u8g2, 2,17,"Hi oled test!");
	ESP_LOGI(TAG, "u8g2_SendBuffer");
	u8g2_SendBuffer(&u8g2);

	ESP_LOGI(TAG, "All done!");

	vTaskDelay(2000/ portTICK_PERIOD_MS);

    u8g2_DrawStr(&u8g2, 2,48,"Hi oled test!");
	u8g2_SendBuffer(&u8g2);

	vTaskDelete(NULL);
}

void MyTempTask(void *pvParam)
{
//	oled = SSD1306(21, 23);
	oled.clear();
	oled.print("DS18B20 init");
	DS18B20 sensors(13);
	std::string txt = "";
	while (1)
	{
		txt = "";
		sensors.FindDevices();
		ESP_LOGD(tag, "Devices count: %i", sensors.Count());
		char buff[32];
		for (int i = 0; i < sensors.Count(); i++)
		{
			float temp = sensors.TempC(i);
			sprintf(buff, "%i --> %0.4f C\n", i + 1, temp);
			ESP_LOGD(tag, "Dev %i, addr: %s, temp: %0.4f", (i + 1), sensors.Dev(i).c_str(), temp);
			txt += std::string(buff);
			oled.print(txt);
		}
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}

void Task_U8g2(void *ignore)
{
	static U8G2 u8g2(PIN_SDA, PIN_SCL, 0x3C);
	u8g2.initDisplay();
	u8g2.setPowerSave(0);
	u8g2.clearBuffer();
	u8g2.drawBox(0, 26, 80,6);
	u8g2.drawFrame(0,26,100,6);
	u8g2.setFont(u8g2_font_ncenB14_tr);
	u8g2.drawStr(2,17,"Hi wito!");

	u8g2.sendBuffer();


//	u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
//
//	ESP_LOGI(TAG, "u8g2_SetPowerSave");
//	u8g2_SetPowerSave(&u8g2, 0); // wake up display
//	ESP_LOGI(TAG, "u8g2_ClearBuffer");
//	u8g2_ClearBuffer(&u8g2);
//	ESP_LOGI(TAG, "u8g2_DrawBox");
//	u8g2_DrawBox(&u8g2, 0, 26, 80,6);
//	u8g2_DrawFrame(&u8g2, 0,26,100,6);
//
//	ESP_LOGI(TAG, "u8g2_SetFont");
//    u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
//	ESP_LOGI(TAG, "u8g2_DrawStr");
//    u8g2_DrawStr(&u8g2, 2,17,"Hi nkolban!");
//	ESP_LOGI(TAG, "u8g2_SendBuffer");
//	u8g2_SendBuffer(&u8g2);

	vTaskDelete(NULL);
}

extern "C"
{
void app_main(void);
}

void app_main(void)
{
	oled = SSD1306(PIN_SCL, PIN_SDA);
	oled.clear();
	oled.print("START");
//	vTaskDelay(1000/portTICK_PERIOD_MS);
	delay(1000);

	for (int i = 1; i <= 8; i++)
	{
		oled.clear();
		oled.fillBuffer(i);
		oled.drawBuffer();
		delay(500);
	}

	for (int i = 9; i >= 0; i--)
	{
		char c[1];
		sprintf(c, "%i", i);
		oled.print("Starting in " + std::string(c));
		vTaskDelay(200 / portTICK_PERIOD_MS);
	}
	xTaskCreatePinnedToCore(&MyTempTask, "MyTempTask", 10240, NULL, 5, NULL, 1);

//	xTaskCreatePinnedToCore(&task_test_SSD1306i2c, "task_test_SSD1306i2c", 10240, NULL, 5, NULL, 1);
//	xTaskCreate(&Task_U8g2, "Task_U8g2", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}

