#pragma once

#include <stdbool.h>

#include "app_config.h"

typedef struct {
    int consecutive_wet_samples;
    system_event_t last_reported_state;
} sensor_debounce_state_t;

static inline bool sensor_raw_is_wet(int raw, int threshold)
{
    return raw >= threshold;
}

void sensor_debounce_init(sensor_debounce_state_t *state);
system_event_t sensor_debounce_update(sensor_debounce_state_t *state,
                                      int raw,
                                      int threshold,
                                      int required_samples,
                                      bool *changed);
