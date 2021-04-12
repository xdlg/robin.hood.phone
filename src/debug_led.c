#include "debug_led.h"

#include <avr/io.h>

void debug_led_init()
{
    // Set PB2 as output
    DDRB |= (1 << DDB2);

    debug_led_set(false);
}

void debug_led_set(bool is_on)
{
    if (is_on) {
        PORTB &= ~(1 << PB2);
    } else {
        PORTB |= (1 << PB2);
    }
}

void debug_led_toggle()
{
    PINB |= (1 << PINB2);
}
