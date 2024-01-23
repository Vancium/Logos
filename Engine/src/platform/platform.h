#pragma once

#include "../defines.h"


// Platform specific state, contains window handle, platform specific functions etc
// type is detemened in specific platform implementation

typedef struct  platform_state {
    void* internal_state;
} platform_state;

LAPI b8 platform_create(
    platform_state* state,
    const char* name,
    i32 x,
    i32 y,
    i32 width,
    i32 height
);

LAPI void platform_destroy(platform_state* state);

LAPI b8 platform_pump_messages(platform_state* state);

void* platform_allocate(u64 size, b8 aligned);
void platform_free(void* block, b8 aligned);
void* platform_zero_memory(void* block, u64 size);
void* platform_copy_memory(void* dest, const void* src, u64 size);
void* platform_set_memory(void* dest, i32 value, u64 size);

void platform_console_write(const char* message, u8 color);
void platform_console_write_error(const char* message, u8 color);

f64 platform_get_absolute_time();

void platform_sleep(u64 ms);
