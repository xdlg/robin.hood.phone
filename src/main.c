#include "audio_board.h"
#include "clock.h"
#include "debug_led.h"
#include "dial.h"

#include "AVR-UART-lib/usart.h"

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

// Initializes the hardware
void init();

// Starts infinite error loop
void error_state();

int main()
{
    init();

    bool success = false;
    const uint32_t timeout_ms = 2000;
    uint32_t then_ms = clock_now_ms();

    while ((clock_now_ms() - then_ms) < timeout_ms) {
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

    then_ms = clock_now_ms();

    while (audio_board_is_playing()) {
        const uint32_t sampling_time_ms = 5;
        uint32_t now_ms = clock_now_ms();

        if ((now_ms - then_ms) >= sampling_time_ms) {
            then_ms = now_ms;
            dial_monitor_activity();
            if (dial_is_dialling()) {
                audio_board_stop_playback();
                break;
            }
        }
    }

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
    clock_init();
    debug_led_init();
    dial_init();
    audio_board_init();
    sei();
}

void error_state()
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
