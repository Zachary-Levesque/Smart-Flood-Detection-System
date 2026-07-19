#pragma once

#include "app_config.h"

void event_log_init(void);
void event_log_dump_task_start(void);
void event_log_record(system_event_t event);
void event_log_dump_to_serial(void);
const char *system_event_to_string(system_event_t event);
