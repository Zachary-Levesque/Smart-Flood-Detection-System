#pragma once

#include <stdbool.h>

// Initializes NVS, WiFi stack, and connects to the configured AP.
// Handles automatic reconnection on disconnect. Non-blocking: the
// rest of the system (sensor + local alarm) does not depend on this
// having succeeded.
void wifi_manager_start(void);

// Returns true if currently connected to WiFi.
bool wifi_manager_is_connected(void);
