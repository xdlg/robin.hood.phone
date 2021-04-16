#include "audio_board.h"
#include "clock.h"
#include "debug_led.h"
#include "dial.h"

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

// Initializes the hardware.
// Returns true on success.
bool init();

// Waits for dialled number.
// Returns false on error.
bool wait_for_dial();

// Plays the busy tone.
// Returns true if the tone has been found and entirely played back, false otherwise.
bool busy();

// Starts infinite error loop.
void error();

// Starts infinite idle loop.
void idle();

int main()
{
    if (!init()) error();
    if (!wait_for_dial()) error();
    if (!busy()) error();
    idle();
}

bool init()
{
    clock_init();
    debug_led_init();
    dial_init();
    audio_board_init();
    debug_led_set(true);
    sei();

    bool success = false;
    const uint32_t timeout_ms = 1000;
    uint32_t then_ms = clock_now_ms();

    while ((clock_now_ms() - then_ms) < timeout_ms) {
        if (!success && audio_board_has_started()) success = true;
    }

    return success;
}

bool wait_for_dial()
{
    if (!audio_board_play_dial_tone()) { return false; }

    dial_state_t current_dial_state = DIAL_STATE_WAITING;
    uint32_t then_ms = clock_now_ms();

    while (audio_board_is_playing() || (current_dial_state == DIAL_STATE_DIALLING)) {
        const uint32_t sampling_time_ms = 5;
        uint32_t now_ms = clock_now_ms();

        if ((now_ms - then_ms) >= sampling_time_ms) {
            then_ms = now_ms;
            current_dial_state = dial_state();
            if (audio_board_is_playing() && (current_dial_state == DIAL_STATE_DIALLING)) {
                audio_board_stop_playback();
            }
        }
    }

    if (current_dial_state == DIAL_STATE_FINISHED) {
        if (audio_board_play_file(dial_number())) {
            while (audio_board_is_playing()) {}

            // Short delay for better user experience
            const uint32_t hangup_delay_ms = 2000;
            then_ms = clock_now_ms();
            while ((clock_now_ms() - then_ms) < hangup_delay_ms) {}
        };
    }

    return true;
}

bool busy()
{
    if (!audio_board_play_busy_tone()) { return false; }
    while (audio_board_is_playing()) {}
    return true;
}

void error()
{
    uint32_t then_ms = clock_now_ms();

    while (true) {
        const uint32_t timeout_ms = 250;
        uint32_t now_ms = clock_now_ms();

        if ((now_ms - then_ms) >= timeout_ms) {
            then_ms = now_ms;
            debug_led_toggle();
        }
    }
}

void idle()
{
    debug_led_set(false);
    while (true) {}
}
