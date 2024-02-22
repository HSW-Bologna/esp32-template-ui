#ifndef BACKLIGHT_H_INCLUDED
#define BACKLIGHT_H_INCLUDED


#include <stdint.h>


void tft_backlight_init(void);
void tft_backlight_set(uint8_t percentage);


#endif
