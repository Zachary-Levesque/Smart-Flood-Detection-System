#include "sensor_logic.h"

void sensor_debounce_init(sensor_debounce_state_t *state)
{
    state->consecutive_wet_samples = 0;
    state->last_reported_state = EVENT_WATER_NORMAL;
}

system_event_t sensor_debounce_update(sensor_debounce_state_t *state,
                                      int raw,
                                      int threshold,
                                      int required_samples,
                                      bool *changed)
{
    if (sensor_raw_is_wet(raw, threshold)) {
        state->consecutive_wet_samples++;
    } else {
        state->consecutive_wet_samples = 0;
    }

    system_event_t current_state =
        (state->consecutive_wet_samples >= required_samples)
            ? EVENT_WATER_DETECTED
            : EVENT_WATER_NORMAL;

    *changed = (current_state != state->last_reported_state);
    if (*changed) {
        state->last_reported_state = current_state;
    }

    return current_state;
}
