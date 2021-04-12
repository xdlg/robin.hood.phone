#include "audio_board.h"
#include "debug_led.h"
#include "timer.h"

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>

// Initializes the hardware
void init();

// Starts infinite error loop
void error_state();

int main()
{
    init();

    bool success = false;
    timer_start(1000);

    while (!timer_has_timed_out()) {
        if (audio_board_has_started()) {
            success = true;
        }
    }

    if (!success) {
        error_state();
    }

    debug_led_set(true);

    if (!audio_board_play_dial_tone()) {
        error_state();
    }
    while (audio_board_is_playing()) {}

    if (!audio_board_play_busy_tone()) {
        error_state();
    }
    while (audio_board_is_playing()) {}

    debug_led_set(false);
    /// @todo Go to sleep
    while (true) {}
}

void init()
{
    debug_led_init();
    audio_board_init();
    sei();
}

void error_state()
{
    while (true) {
        debug_led_toggle();
        _delay_ms(250);
    }
}
