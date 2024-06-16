#include "FreeRTOS.h"
#include "task.h"
#include "esp_log.h"
#include <stdint.h>
#include <lvgl.h>
#include <X11/X.h>
#include "model/model.h"
#include "controller/controller.h"


static void on_close_cb(lv_event_t * e);


static const char *TAG = "Main";
static uint8_t terminated = 0;


void app_main(void *arg) {
    model_t model;
    (void)arg;

    model_init(&model);
    // view_init(LV_BUFFER_SIZE, display_flush, touch_read);

    lv_init();

    /* initialize X11 display driver */
    lv_display_t *disp = lv_x11_window_create("LVGL X11 Simulation", 480, 320);
    lv_display_add_event_cb(disp, on_close_cb, LV_EVENT_DELETE, NULL);
    /* initialize X11 input drivers (for keyboard, mouse & mousewheel) */
    LV_IMG_DECLARE(img_mouse);
    lv_x11_inputs_create(disp, &img_mouse);
    /* set optional window close callback to enable application cleanup and exit */
    lv_display_add_event_cb(disp, on_close_cb, LV_EVENT_DELETE, disp);

    controller_init(&model);

    ESP_LOGI(TAG, "Begin main loop");
    while (!terminated) {
        view_manage();
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelete(NULL);
}


static void on_close_cb(lv_event_t * e)
{
    terminated = 1;
}
