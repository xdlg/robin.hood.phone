#include "audio_board.h"
#include "debug_led.h"
#include "dial.h"
#include "clock.h"

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

    // bool success = false;
    // const uint32_t timeout_ms = 1000;
    // uint32_t then_ms = clock_now_ms();
    //
    // while ((clock_now_ms() - then_ms) < timeout_ms) {
    //     if (audio_board_has_started()) {
    //         success = true;
    //     }
    // }

    // if (!success) {
    //     error_state();
    // }

    debug_led_set(true);
    //
    // if (!audio_board_play_dial_tone()) {
    //     error_state();
    // }
    // while (audio_board_is_playing()) {}
    //
    // if (!audio_board_play_busy_tone()) {
    //     error_state();
    // }
    // while (audio_board_is_playing()) {}
    //
    // debug_led_set(false);
    /// @todo Go to sleep
    // uint8_t digit = 0;

    uint32_t then_ms = clock_now_ms();

    while (true) {
        const uint32_t sampling_time_ms = 10;
        uint32_t now_ms = clock_now_ms();

        if ((now_ms - then_ms) >= sampling_time_ms) {
            then_ms = now_ms;
            // As soon as we have a falling edge, we can move on
            if (!dial_poll_dialling_pin()) {
                break;
            }
        }
    }

    debug_led_set(false);

    uint8_t last_digit = dial_last_digit();
    uart_putint(last_digit);
    uart_puts("\r\n");

    while (true) {
    }
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
