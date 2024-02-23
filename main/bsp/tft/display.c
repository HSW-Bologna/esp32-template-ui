#include <esp_log.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_ili9488.h>
#include <driver/spi_master.h>
#include "sdkconfig.h"
#include "esp_idf_version.h"

#include "bsp/hardwareprofile.h"


static bool on_color_trans_done(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata,
                                void *user_ctx);


static const int          DISPLAY_COMMAND_BITS   = 8;
static const int          DISPLAY_PARAMETER_BITS = 8;
static const unsigned int DISPLAY_REFRESH_HZ     = 40000000;
static const int          DISPLAY_SPI_QUEUE_LEN  = 10;
static const char        *TAG                    = "Display";


static esp_lcd_panel_io_handle_t lcd_io_handle  = NULL;
esp_lcd_panel_handle_t           lcd_handle     = NULL;
static void (*notify_transaction_done_cb)(void) = NULL;


void tft_display_init(uint32_t buffer_size, void (*notify_transaction_done)(void)) {
    notify_transaction_done_cb = notify_transaction_done;

    const esp_lcd_panel_io_spi_config_t io_config = {
        .cs_gpio_num         = HAP_CS_D,
        .dc_gpio_num         = HAP_D,
        .spi_mode            = 0,
        .pclk_hz             = DISPLAY_REFRESH_HZ,
        .trans_queue_depth   = DISPLAY_SPI_QUEUE_LEN,
        .on_color_trans_done = on_color_trans_done,
        .user_ctx            = NULL,
        .lcd_cmd_bits        = DISPLAY_COMMAND_BITS,
        .lcd_param_bits      = DISPLAY_PARAMETER_BITS,
        .flags               = {
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
            .dc_as_cmd_phase = 0, .dc_low_on_data = 0, .octal_mode = 0, .lsb_first = 0
#else
            .dc_low_on_data = 0, .octal_mode = 0, .sio_mode = 0, .lsb_first = 0, .cs_high_active = 0
#endif
        }};

    const esp_lcd_panel_dev_config_t lcd_config = {
        .reset_gpio_num = HAP_RESET_D,
        .rgb_ele_order  = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 18,
        .flags          = {.reset_active_high = 0},
        .vendor_config  = NULL,
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST, &io_config, &lcd_io_handle));

    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9488(lcd_io_handle, &lcd_config, buffer_size, &lcd_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(lcd_handle, false));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(lcd_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(lcd_handle, true, false));
    ESP_ERROR_CHECK(esp_lcd_panel_set_gap(lcd_handle, 0, 0));
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
    ESP_ERROR_CHECK(esp_lcd_panel_disp_off(lcd_handle, false));
#else
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(lcd_handle, true));
#endif

    ESP_LOGI(TAG, "Initialized");
}


void tft_display_flush(int32_t offsetx1, int32_t offsety1, int32_t offsetx2, int32_t offsety2, void *data) {
    esp_lcd_panel_draw_bitmap(lcd_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, data);
}


static bool on_color_trans_done(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata,
                                void *user_ctx) {
    if (notify_transaction_done_cb) {
        notify_transaction_done_cb();
    }
    return false;
}
