#include "dial.h"
// #include "debug_led.h"
#include "clock.h"

// #include <avr/interrupt.h>
#include <avr/io.h>
// #include <util/atomic.h>

static volatile bool is_dialling;
static volatile uint8_t digit;

void dial_init()
{
    // is_dialling = false;
    // digit = 0xFF;

    // Set PD2/INT0 as input with pull-up (connected to the "dialling in progress" pin)
    DDRD &= ~(1 << DDD2);
    PORTD |= (1 << PORTD2);

    // Set PD3/INT1 as input with pull-up (connected to the "number" pin)
    DDRD &= ~(1 << DDD3);
    PORTD |= (1 << PORTD3);

    // Enable rising edge interrupt on dialling in progress
    // EICRA |= (1 << ISC00);
    // EICRA |= (1 << ISC01);
    // EIMSK |= (1 << INT0);

    // Enable rising edge interrupt on digit signal
    // EICRA |= (1 << ISC10);
    // EICRA |= (1 << ISC11);
    // EIMSK |= (1 << INT1);
}

uint8_t dial_last_digit()
{
    uint8_t digit = 0;
    bool old_digit_pin = false;

    const uint32_t digit_timeout_ms = 2000;
    uint32_t last_edge_time_ms = clock_now_ms();
    uint32_t then_ms = last_edge_time_ms;

    while ((clock_now_ms() - last_edge_time_ms) < digit_timeout_ms) {
        const uint32_t sampling_time_ms = 5;
        uint32_t now_ms = clock_now_ms();

        if ((now_ms - then_ms) >= sampling_time_ms) {
            then_ms = now_ms;
            bool new_digit_pin = dial_poll_digit_pin();

            // Rising edge
            if (!old_digit_pin && new_digit_pin) {
                digit++;
                last_edge_time_ms = clock_now_ms();
            }

            old_digit_pin = new_digit_pin;
        }
    }

    if (digit == 0) {
        return INVALID_DIGIT;
    } else if (digit == 10) {
        return 0;
    } else {
        return digit;
    }
}

bool dial_poll_dialling_pin()
{
    static const uint8_t counter_max = 5;
    static uint8_t counter = 0;
    static bool debounced_pin = true;
    bool hardware_pin = (PIND & (1 << PIND2));

    if (debounced_pin == hardware_pin) {
        counter = 0;
    } else if (++counter == counter_max) {
        debounced_pin = hardware_pin;
        counter = 0;
    } else {
        // Do nothing
    }

    return debounced_pin;
}

bool dial_poll_digit_pin()
{
    static const uint8_t counter_max = 5;
    static uint8_t counter = 0;
    static bool debounced_pin = false;
    bool hardware_pin = (PIND & (1 << PIND3));

    if (debounced_pin == hardware_pin) {
        counter = 0;
    } else if (++counter == counter_max) {
        debounced_pin = hardware_pin;
        counter = 0;
    } else {
        // Do nothing
    }

    return debounced_pin;
}

// uint8_t dial_wait_for_digit()
// {
//     digit = 0xFF;
//
//     // Enable rising edge interrupt on digit signal
//     EICRA |= (1 << ISC10);
//     EICRA |= (1 << ISC11);
//     EIMSK |= (1 << INT1);
//
//     static const uint32_t timeout_ms = 2000;
//     timer_start(timeout_ms);
//     while (!timer_has_timed_out()) {}
//
//     // Disable interrupt
//     EIMSK |= (1 << INT1);
//
//     ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
//     {
//         is_dialling = false;
//     }
//
//     return (digit == 10) ? 0 : digit;
// }

// ISR(INT0_vect)
// {
//     is_dialling = true;
// }

// ISR(INT1_vect)
// {
    // digit++;
    // if (digit == 0xFF) {
    //     digit = 1;
    // } else {
    //     digit++;
    // }
// }
