#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED


#include <lvgl.h>


void view_init(uint32_t buffer_size,
               void (*display_flush)(struct _lv_display_t *display, const lv_area_t *area, unsigned char *px_map),
               void (*touch_read)(struct _lv_indev_t *indev, lv_indev_data_t *data));
void view_manage(void);
void view_notify_lvgl_flush_ready(void);


#endif
