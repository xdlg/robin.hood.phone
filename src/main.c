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
    timer_start(2000);

    while (!timer_has_timed_out()) {
        if (audio_board_has_started()) {
            success = true;
        }
    }

    if (success) {
        debug_led_set(true);
        audio_board_play_dial_tone();
        audio_board_is_done_playing();
        audio_board_play_busy_tone();
        audio_board_is_done_playing();
        debug_led_set(false);
        while (true) {}
    }

    error_state();
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
