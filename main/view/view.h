#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED


#include <lvgl.h>


void view_init(uint32_t buffer_size,
               void (*display_flush)(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map),
               void (*touch_read)(struct _lv_indev_drv_t *indev_drv, lv_indev_data_t *data));
void view_manage(void);
void view_notify_lvgl_flush_ready(void);


#endif
