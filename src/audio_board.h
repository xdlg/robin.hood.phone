#pragma once

#include <stdbool.h>

// Initializes the audio board
void audio_board_init();

// Returns true if the audio board has started successfully
bool audio_board_has_started();

bool audio_board_is_done_playing();

bool audio_board_play_dial_tone();

bool audio_board_play_busy_tone();
