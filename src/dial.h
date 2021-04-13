#pragma once

#include <stdint.h>
#include <stdbool.h>

#define INVALID_DIGIT 0xFF

void dial_init();

bool dial_is_dialling();

uint8_t dial_last_digit();

bool dial_poll_dialling_pin();
bool dial_poll_digit_pin();
