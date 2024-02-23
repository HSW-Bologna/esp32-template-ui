#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include "lvgl.h"

#include "model/model.h"
#include "view/view.h"
#include "controller/controller.h"
#include "bsp/tft/backlight.h"
#include "bsp/tft/display.h"
#include "bsp/tft/touch.h"
#include "bsp/system.h"


static void display_flush(lv_display_t *display, const lv_area_t *area, unsigned char *px_map);
static void touch_read(struct _lv_indev_t *indev, lv_indev_data_t *data);


static const char *TAG = "Main";

static const int    DISPLAY_HORIZONTAL_PIXELS = 480;
static const size_t LV_BUFFER_SIZE            = DISPLAY_HORIZONTAL_PIXELS * 25;


void app_main(void) {
    model_t model;

    system_init();
    tft_backlight_init();
    tft_backlight_set(100);
    tft_display_init(LV_BUFFER_SIZE, view_notify_lvgl_flush_ready);
    tft_touch_init();

    model_init(&model);
    view_init(LV_BUFFER_SIZE, display_flush, touch_read);
    controller_init(&model);

    lv_obj_t *obj = lv_btn_create(lv_scr_act());
    lv_obj_set_size(obj, 100, 100);
    lv_obj_center(obj);

    lv_obj_t *lbl = lv_label_create(obj);
    lv_label_set_text(lbl, "Ciao");

    ESP_LOGI(TAG, "Begin main loop");
    for (;;) {
        view_manage();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


static void display_flush(lv_display_t *display, const lv_area_t *area, unsigned char *px_map) {
    (void)display;
    tft_display_flush(area->x1, area->y1, area->x2, area->y2, px_map);
}


static void touch_read(struct _lv_indev_t *indev_drv, lv_indev_data_t *data) {
    (void)indev_drv;

    int32_t x = 0;
    int32_t y = 0;

    if (tft_touch_read(&y, &x)) {
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }

    data->point.x = x;
    data->point.y = y;
}
