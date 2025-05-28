#include "config.h"
#include "esp_inmp441.h"
#include "esp_oled.h"

void i2s_install(void){
    //ESP_LOGI(TAG_INMP441, "Cau hinh kenh i2s...");
    oled_display_text("I2S installing...");
    //Cau hinh kenh rx cho i2s
    i2s_chan_config_t chan_cfg = {
        .id = I2S_NUM,
        .role = I2S_ROLE_MASTER,
        .dma_desc_num = dmaDesc, //6 * 128 = 768
        .dma_frame_num = dmaLen, //128 bytes
        .auto_clear = true, 
    };

    //Khoi tao RX channel va ktr loi
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, NULL, &rx_channel));

    //Cau hinh che do chuan i2s, API moi
    i2s_std_config_t std_cfg = {
        //Cau hinh clock
        .clk_cfg = {
            .sample_rate_hz = SAMPLE_RATE, //Tan so lay mau
            .clk_src = I2S_CLK_SRC_DEFAULT, //Nguon clock mac dinh
            /**
             * @param mclk_multiple cang tang thi do nhieu (jitter) cua CLK va WS cang giam => Do chinh xac du lieu cang cao    
             * Day la boi so cua master clock doi voi tan so lay mau 
             */
            .mclk_multiple = I2S_MCLK_MULTIPLE_384, //Boi cua 3 (384,768,..) de phu hop voi du lieu 24 bit
        },  
        //Cau hinh du lieu trong 1 frame
        .slot_cfg = {
            /**
             * @note De 32-bit width thi moi chu ki dem duoc 10 mau
             * nhung khi de xuong 24-bit width thi dem len 13 mau ???
             */
            .data_bit_width = I2S_DATA_BIT_WIDTH_32BIT, //So bit moi du lieu
            .slot_mode = I2S_SLOT_MODE_MONO, //Che do mono (thu/phat 1 kenh)
            .slot_bit_width = I2S_SLOT_BIT_WIDTH_32BIT, //So bit moi slot (moi kenh)
            .slot_mask = I2S_STD_SLOT_LEFT //Kenh du lieu trai
        },      
        //Cau hinh GPIO
        .gpio_cfg = {
            .bclk = I2S_SCK_PIN,
            .ws = I2S_WS_PIN, 
            .dout = I2S_PIN_NO_CHANGE, //Khong can output data
            .din = I2S_SD_PIN, //GPIO cho du lieu input
        }
    };
    //Khoi tao che do chuan 
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(rx_channel, &std_cfg));
    //Bat kenh RX 
    ESP_ERROR_CHECK(i2s_channel_enable(rx_channel));
    //ESP_LOGI(TAG_INMP441, "I2S da duoc cau hinh thanh cong !");
}

void readINMP441data_task(void *pvParameters){ 
    i2s_install();
    //ESP_LOGI(TAG_INMP441, "Bat dau doc du lieu tu INMP441...");
    oled_display_text("INMP441 reading...");
    size_t bytes_read;
    while(true){
        vTaskDelay(1);
        esp_err_t ret = i2s_channel_read(rx_channel, &buffer32, sizeof(buffer32), &bytes_read, 500);
        if(ret == ESP_ERR_TIMEOUT){
            ESP_LOGE(TAG_INMP441, "Timeout xay ra, bo qua frame loi:  %s", esp_err_to_name(ret));
            continue;
        } else if(ret != ESP_OK){   
            ESP_LOGE(TAG_INMP441, "Loi khong xac dinh: %s", esp_err_to_name(ret));
            break;
        }
        // Khi da nhan duoc du lieu tu I2S 768 bytes (192 mau), thuc hien xu ly du lieu
        int samplesRead = bytes_read / sizeof(int32_t); //Số mẫu đọc được (1 kênh) || dữ liệu int32_t = 4 bytes 
        for(size_t i = 0; i < samplesRead; i++){
            buffer16[i] = (int16_t)(buffer32[i] >> 8); //Raw data
            printf("\n%d", buffer16[i]);
        }
    }  
}