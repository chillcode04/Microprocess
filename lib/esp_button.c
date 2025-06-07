#include "config.h"
#include "esp_button.h"

const char *TAG_BUTTON = "BUTTON";

TimerHandle_t debounce_timer1;
TimerHandle_t debounce_timer2;

uint8_t led_state1;
uint8_t led_state2;
bool FLAG_INMP441 = false; // Cờ để kiểm tra xem INMP441 có đang hoạt động hay không
bool FLAG_MAX30102 = false; // Cờ để kiểm tra xem MAX30102 có đang hoạt động hay không

void IRAM_ATTR button_isr_handler(void *arg)
{
    int gpio_num = (int)(intptr_t)arg;
    if (gpio_num == BUTTON_PIN1)
    {
        xTimerResetFromISR(debounce_timer1, NULL);
    }
    else if (gpio_num == BUTTON_PIN2)
    {
        xTimerResetFromISR(debounce_timer2, NULL);
    }
}

/** Callback debounce cho nút 1 */
void debounce_timer_callback1(TimerHandle_t Timer)
{
    if (gpio_get_level(BUTTON_PIN1) == 0)
    {
        ESP_LOGI(TAG_BUTTON, "Button 1 Pressed!");
        FLAG_INMP441 = true;
    }
}

/** Callback debounce cho nút 2 */
void debounce_timer_callback2(TimerHandle_t Timer)
{
    if (gpio_get_level(BUTTON_PIN2) == 0)
    {
        ESP_LOGI(TAG_BUTTON, "Button 2 Pressed!");
        FLAG_MAX30102 = true;
    }
}


void config_button()
{
    // Cấu hình nút nhấn 1
    gpio_set_direction(BUTTON_PIN1, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN1, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(BUTTON_PIN1, GPIO_INTR_NEGEDGE);

    // Cấu hình nút nhấn 2
    gpio_set_direction(BUTTON_PIN2, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN2, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(BUTTON_PIN2, GPIO_INTR_NEGEDGE);


    // Tạo timer debounce
    debounce_timer1 = xTimerCreate("debounce_timer1", pdMS_TO_TICKS(DEBOUNCE_TIME_MS), pdFALSE, NULL, debounce_timer_callback1);
    debounce_timer2 = xTimerCreate("debounce_timer2", pdMS_TO_TICKS(DEBOUNCE_TIME_MS), pdFALSE, NULL, debounce_timer_callback2);
    // Cài đặt ISR
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN1, button_isr_handler, (void *)(intptr_t)BUTTON_PIN1);
    gpio_isr_handler_add(BUTTON_PIN2, button_isr_handler, (void *)(intptr_t)BUTTON_PIN2);

    ESP_LOGI(TAG_BUTTON, "2 Buttons ISR with Debounce Installed!");
}