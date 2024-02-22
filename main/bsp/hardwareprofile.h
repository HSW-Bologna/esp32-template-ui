#ifndef HARDWAREPROFILE_H_INCLUDED
#define HARDWAREPROFILE_H_INCLUDED

#include <driver/gpio.h>

/*
 * Definizioni dei pin da utilizzare
 */

#define HAP_RETRO   GPIO_NUM_10
#define HAP_RESET_T GPIO_NUM_4
#define HAP_CS_D    GPIO_NUM_5
#define HAP_D       GPIO_NUM_6
#define HAP_CLK_D   GPIO_NUM_7
#define HAP_DOUT_D  GPIO_NUM_8
#define HAP_RESET_D GPIO_NUM_9
#define HAP_SDA     GPIO_NUM_1
#define HAP_SCL     GPIO_NUM_2

#endif
