#ifndef TFT_DISPLAY_H_INCLUDED
#define TFT_DISPLAY_H_INCLUDED


#include <stdint.h>


void tft_display_init(uint32_t buffer_size, void (*notify_transaction_done)(void));
void tft_display_flush(int32_t offsetx1, int32_t offsety1, int32_t offsetx2, int32_t offsety2, void *data);


#endif
