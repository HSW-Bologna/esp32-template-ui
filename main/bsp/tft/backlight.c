#include "driver/ledc.h"
#include <esp_log.h>

#include "bsp/hardwareprofile.h"
#include "backlight.h"


static const ledc_mode_t      BACKLIGHT_LEDC_MODE             = LEDC_LOW_SPEED_MODE;
static const ledc_channel_t   BACKLIGHT_LEDC_CHANNEL          = LEDC_CHANNEL_0;
static const ledc_timer_t     BACKLIGHT_LEDC_TIMER            = LEDC_TIMER_1;
static const ledc_timer_bit_t BACKLIGHT_LEDC_TIMER_RESOLUTION = LEDC_TIMER_10_BIT;
static const uint32_t         BACKLIGHT_LEDC_FRQUENCY         = 5000;
static const char            *TAG                             = "Backlight";


void tft_backlight_init(void) {
    const ledc_channel_config_t LCD_backlight_channel = {
        .gpio_num   = (gpio_num_t)HAP_RETRO,
        .speed_mode = BACKLIGHT_LEDC_MODE,
        .channel    = BACKLIGHT_LEDC_CHANNEL,
        .intr_type  = LEDC_INTR_DISABLE,
        .timer_sel  = BACKLIGHT_LEDC_TIMER,
        .duty       = 0,
        .hpoint     = 0,
        .flags      = {.output_invert = 0},
    };
    const ledc_timer_config_t LCD_backlight_timer = {
        .speed_mode      = BACKLIGHT_LEDC_MODE,
        .duty_resolution = BACKLIGHT_LEDC_TIMER_RESOLUTION,
        .timer_num       = BACKLIGHT_LEDC_TIMER,
        .freq_hz         = BACKLIGHT_LEDC_FRQUENCY,
        .clk_cfg         = LEDC_AUTO_CLK,
    };
    ESP_LOGI(TAG, "Initializing LEDC for backlight pin: %d", HAP_RETRO);

    ESP_ERROR_CHECK(ledc_timer_config(&LCD_backlight_timer));
    ESP_ERROR_CHECK(ledc_channel_config(&LCD_backlight_channel));
}


void tft_backlight_set(uint8_t percentage) {
    if (percentage > 100) {
        percentage = 100;
    }
    ESP_LOGI(TAG, "Setting backlight to %d%%", percentage);

    // LEDC resolution set to 10bits, thus: 100% = 1023
    uint32_t duty_cycle = (1023 * percentage) / 100;
    ESP_ERROR_CHECK(ledc_set_duty(BACKLIGHT_LEDC_MODE, BACKLIGHT_LEDC_CHANNEL, duty_cycle));
    ESP_ERROR_CHECK(ledc_update_duty(BACKLIGHT_LEDC_MODE, BACKLIGHT_LEDC_CHANNEL));
}
