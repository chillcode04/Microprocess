#ifndef CONFIG_H
#define CONFIG_H
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include <inttypes.h>
#include "driver/i2c.h"

//----------------------------------------INMP441-------------------------------------------------//
//Thu vien I2S
#include <driver/i2s_std.h> //API moi <- Dung cai nay
#include <driver/i2s_types_legacy.h>
#include "driver/i2s_types.h"
#include "driver/i2s_common.h"

#define TAG "INMP441"

//Pin cau hinh INMP441 (I2S Mode)
#define I2S_NUM I2S_NUM_0
#define I2S_SCK_PIN 32
#define I2S_WS_PIN 25
#define I2S_SD_PIN 33
#define SAMPLE_RATE 500 //Tan so lay mau 4000Hz

/**
 * @note Sample rate cang cao thi dmaLen cung cang cao de tranh mat mau
 * @param dmaLen Tang len neu du lieu bi mat mau, CPU bi ngat nhieu ma khong quan tam den latency
 * ->Cang lon thi buffer_durations cang lau->Cang it mat mau->CPU xu ly it viec hon->Tre lai cao hon
 * @param dmaDesc So bo dac ta DMA, moi bo co the luu tru so byte = dmaLen 
 */
#define dmaDesc 6 //Bo dac ta DMA
#define dmaLen 128 //So bytes cua moi buffer
#define DMA_BUFFER_SIZE (dmaLen * dmaDesc) //So bytes cua buffer DMA cung cap cho = 768　

extern i2s_chan_handle_t rx_channel; 
//Buffer de luu tru du lieu doc duoc tu buffer DMA
//Chuyen doi tu byte DMA sang so luong mau cua moi buffer 
extern int16_t buffer16[DMA_BUFFER_SIZE / sizeof(int32_t) * 3 / 2]; //288 samples (576 bytes) de luu duoc 3 bytes sau khi dich cua buffer32, VD: 3 byte cần 2 mẫu, ví dụ: 192 mẫu 3 bytes = 288 mẫu 2 bytes
extern int32_t buffer32[DMA_BUFFER_SIZE / sizeof(int32_t)]; //192 samples (768 bytes) , mỗi mẫu 32-bit (4 bytes)

//-----------------------------------------BUTTON--------------------------------------------------/
#define BUTTON_PIN1 4
#define BUTTON_PIN2 16
#define BUTTON_PIN3 17
#define DEBOUNCE_TIME_MS 50
extern const char *TAG_BUTTON;

extern TimerHandle_t debounce_timer1;
extern TimerHandle_t debounce_timer2;
extern TimerHandle_t debounce_timer3;

extern uint8_t led_state1;
extern uint8_t led_state2;
extern uint8_t led_state3;

extern volatile bool FLAG_RECORD;
extern volatile bool FLAG_WIFI;
extern volatile bool FLAG_GMAIL;
//----------------------------------OLED-------------------------------------------------------/
#define SDA_PIN GPIO_NUM_21
#define SCL_PIN GPIO_NUM_22

#define TAG_OLED "OLED"

//----------------------------------MAX30102---------------------------------------------------//
#define I2C_MASTER_SCL_IO    22
#define I2C_MASTER_SDA_IO    21
#define ADC_RESOLUTION       16384.0f  // 14-bit ADC từ MAX30102

extern TaskHandle_t readMAXTask_handle = NULL;
extern struct max30102_record record;
extern unsigned long red, ir;


#endif