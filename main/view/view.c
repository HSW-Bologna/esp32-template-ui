#include <esp_log.h>
#include <lvgl.h>
#include "bsp/tft/display.h"
#include "bsp/tft/touch.h"
#include "esp_heap_caps.h"
#include "view.h"
#include "services/tick.h"


static const char *TAG = "View";

static lv_disp_t         *lv_display = NULL;
static lv_color_t        *lv_buf_1   = NULL;
static lv_color_t        *lv_buf_2   = NULL;
static lv_disp_draw_buf_t lv_disp_buf;

static const int DISPLAY_HORIZONTAL_PIXELS = 320;
static const int DISPLAY_VERTICAL_PIXELS   = 480;


lv_disp_drv_t      lv_disp_drv;
static lv_indev_t *touch_indev;


void view_init(uint32_t buffer_size,
               void (*display_flush)(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map),
               void (*touch_read)(struct _lv_indev_drv_t *indev_drv, lv_indev_data_t *data)) {
    ESP_LOGI(TAG, "Initializing LVGL");
    lv_init();
    ESP_LOGI(TAG, "Allocating %zu bytes for LVGL buffer", buffer_size * sizeof(lv_color_t));
    lv_buf_1 = (lv_color_t *)heap_caps_malloc(buffer_size * sizeof(lv_color_t), MALLOC_CAP_DMA);
#if USE_DOUBLE_BUFFERING
    ESP_LOGI(TAG, "Allocating %zu bytes for second LVGL buffer", buffer_size * sizeof(lv_color_t));
    lv_buf_2 = (lv_color_t *)heap_caps_malloc(buffer_size * sizeof(lv_color_t), MALLOC_CAP_DMA);
#endif
    ESP_LOGI(TAG, "Creating LVLG display buffer");
    lv_disp_draw_buf_init(&lv_disp_buf, lv_buf_1, lv_buf_2, buffer_size);

    ESP_LOGI(TAG, "Initializing %dx%d display", DISPLAY_HORIZONTAL_PIXELS, DISPLAY_VERTICAL_PIXELS);
    lv_disp_drv_init(&lv_disp_drv);
    lv_disp_drv.hor_res  = DISPLAY_HORIZONTAL_PIXELS;
    lv_disp_drv.ver_res  = DISPLAY_VERTICAL_PIXELS;
    lv_disp_drv.flush_cb = display_flush;
    lv_disp_drv.draw_buf = &lv_disp_buf;
    lv_display           = lv_disp_drv_register(&lv_disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv); /*Basic initialization*/
    indev_drv.long_press_time        = 1500UL;
    indev_drv.long_press_repeat_time = 250UL;
    indev_drv.type                   = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb                = touch_read;
    /*Register the driver in LVGL and save the created input device object*/
    touch_indev = lv_indev_drv_register(&indev_drv);
}


void view_notify_lvgl_flush_ready(void) {
    lv_disp_flush_ready(&lv_disp_drv);
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
