#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"


typedef unsigned long timestamp_t;


static inline __attribute__((always_inline)) timestamp_t tick_time_interval(timestamp_t a, timestamp_t b) {
#define TICK_TYPECHECK(type, x)                                                                                        \
    ({                                                                                                                 \
        type      __dummy;                                                                                             \
        typeof(x) __dummy2;                                                                                            \
        (void)(&__dummy == &__dummy2);                                                                                 \
        1;                                                                                                             \
    })
#define TICK_TIME_AFTER(a, b)                                                                                          \
    (TICK_TYPECHECK(timestamp_t, a) && TICK_TYPECHECK(timestamp_t, b) && ((long)((b) - (a)) < 0))

    if (TICK_TIME_AFTER(a, b))
        return -((timestamp_t)b - (timestamp_t)a);
    else
        return (timestamp_t)b - (timestamp_t)a;

#undef TICK_TYPECHECK
#undef TICK_TIME_AFTER
}


static inline __attribute__((always_inline)) timestamp_t tick_get_ms(void) {
    return (timestamp_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
}


#endif
