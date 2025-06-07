#include "config.h"

void app_main(void)
{
    oled_config();
    oled_display_text("HUY");
    config_button();
    ESP_ERROR_CHECK(i2cdev_init());
    i2s_install();
    printf("I2C device initialized\n");
    while (1)
    {
        if (FLAG_INMP441)
        {
            oled_display_text("   INMP441 ...");
            FLAG_INMP441 = false;
            readINMP441data_task();
        }
        if (FLAG_MAX30102)
        {
            oled_display_text("   MAX30102 ...");
            FLAG_MAX30102 = false;
            max30102_test();
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
