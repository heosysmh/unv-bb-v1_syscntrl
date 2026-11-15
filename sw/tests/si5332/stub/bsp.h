/*
 * Host stub for bsp.h: only provides bsp_uDelay() (counter instead of a real delay).
 */
#pragma once

#include <stdint.h>

extern unsigned long g_delay_us;

/* Like the real BSP, including the trailing semicolon */
#define bsp_uDelay(usec)    g_delay_us += (usec);
