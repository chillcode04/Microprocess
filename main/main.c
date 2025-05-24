#include "config.h"
#include "esp_inmp441.h"
#include "esp_oled.h"
#include "esp_button.h"
#include "max30102.h"

void app_main(void)
{
    oled_config();
    max30102_read();
}
