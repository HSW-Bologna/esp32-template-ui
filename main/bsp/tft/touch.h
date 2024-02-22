#ifndef TOUCH_H_INCLUDED
#define TOUCH_H_INCLUDED


#include <lvgl.h>


void    tft_touch_init(void);
uint8_t tft_touch_read(int32_t *x, int32_t *y);


#endif
