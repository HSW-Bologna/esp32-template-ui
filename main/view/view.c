#include <esp_log.h>
#include <lvgl.h>
#include "bsp/tft/display.h"
#include "bsp/tft/touch.h"
#include "esp_heap_caps.h"
#include "view.h"
#include "services/tick.h"


static const char *TAG = "View";

static lv_color_t *lv_buf_1   = NULL;
static lv_color_t *lv_buf_2   = NULL;

static const int DISPLAY_HORIZONTAL_PIXELS = 480;
static const int DISPLAY_VERTICAL_PIXELS   = 320;


lv_display_t *display;


void view_init(uint32_t buffer_size,
               void (*display_flush)(struct _lv_display_t *display, const lv_area_t *area, unsigned char *px_map),
               void (*touch_read)(struct _lv_indev_t *indev, lv_indev_data_t *data)) {
    ESP_LOGI(TAG, "Initializing LVGL");
    lv_init();

    ESP_LOGI(TAG, "Allocating %zu bytes for LVGL buffer", (size_t)buffer_size * sizeof(lv_color_t));
    lv_buf_1 = (lv_color_t *)heap_caps_malloc(buffer_size * sizeof(lv_color_t), MALLOC_CAP_DMA);

    ESP_LOGI(TAG, "Creating LVGL display buffer");
    display = lv_display_create(DISPLAY_HORIZONTAL_PIXELS, DISPLAY_VERTICAL_PIXELS);
    lv_display_set_buffers(display, lv_buf_1, lv_buf_2, buffer_size, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(display, display_flush);

    /*Register at least one display before you register any input devices*/
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*See below.*/
    lv_indev_set_read_cb(indev, touch_read);         /*See below.*/
}


void view_notify_lvgl_flush_ready(void) {
    lv_disp_flush_ready(display);
}


void view_manage(void) {
    static unsigned long last_invoked_ts = 0;
    unsigned long        timestamp       = tick_get_ms();

    if (last_invoked_ts != timestamp) {
        if (last_invoked_ts > 0) {
            lv_tick_inc(tick_time_interval(last_invoked_ts, timestamp));
        }
        last_invoked_ts = timestamp;
    }

    lv_timer_handler();
}
