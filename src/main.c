#include "audio_board.h"
#include "debug_led.h"
#include "timer.h"

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>

// Initializes the hardware
void init();

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
        while (true) {}
    } else {
        // Error state
        while (true) {
            debug_led_toggle();
            _delay_ms(250);
        }
    }
}

void init()
{
    debug_led_init();
    audio_board_init();
    sei();
}
