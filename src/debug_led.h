#pragma once

#include <stdbool.h>

// Initializes the debug LED
void debug_led_init();

// Turns the debug LED on or off
void debug_led_set(bool is_on);

// Toggles the debug LED
void debug_led_toggle();
