#pragma once

#include "app_state.h"

void sync_service_pull_notes(app_note_t * notes, uint8_t * count, uint8_t max_count);
void sync_service_pull_todos(app_todo_t * todos, uint8_t * count, uint8_t max_count);
