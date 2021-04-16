#pragma once

#include <stdbool.h>

// Initializes the audio board.
void audio_board_init();

// Returns true if the audio board has started successfully.
bool audio_board_has_started();

// Returns true if the audio board is playing a file.
bool audio_board_is_playing();

// Plays the dial tone, returns true if playing successfully.
bool audio_board_play_dial_tone();

// Plays the busy tone, returns true if playing successfully.
bool audio_board_play_busy_tone();

// Plays an OGG file by name, returns true if playing successfully.
// The file name must be null-terminated.
bool audio_board_play_file(const char* file_name);

// Stops playback, returns true on success.
bool audio_board_stop_playback();
