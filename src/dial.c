#include "dial.h"
#include "clock.h"

#include <avr/io.h>

static volatile bool is_dialling;
static volatile uint8_t digit;

// Polls the dialling pin, which indicates that a digit dialling has started.
// Must be called at regular intervals.
static bool dial_poll_dialling_pin();

// Polls the digit pin, which indicates the digit being dialled.
// Must be called at regular intervals.
static bool dial_poll_digit_pin();

void dial_init()
{
    // Set PD2/INT0 as input with pull-up (connected to the "dialling in progress" pin)
    DDRD &= ~(1 << DDD2);
    PORTD |= (1 << PORTD2);

    // Set PD3/INT1 as input with pull-up (connected to the "number" pin)
    DDRD &= ~(1 << DDD3);
    PORTD |= (1 << PORTD3);
}

void dial_monitor_activity()
{
    is_dialling = !dial_poll_dialling_pin();
    dial_poll_digit_pin();
}

bool dial_is_dialling()
{
    return is_dialling;
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

static bool dial_poll_dialling_pin()
{
    static uint8_t counter = 0;
    // Number of consecutive signal samples required to consider the signal stable
    static const uint8_t n_stable_samples = 5;
    // The pin is normally high and goes low when the dial is turned counter-clockwise
    static bool debounced_pin = true;
    bool hardware_pin = (PIND & (1 << PIND2));

    if (debounced_pin == hardware_pin) {
        counter = 0;
    } else if (++counter == n_stable_samples) {
        debounced_pin = hardware_pin;
        counter = 0;
    } else {
        // Do nothing
    }

    return debounced_pin;
}

static bool dial_poll_digit_pin()
{
    static uint8_t counter = 0;
    // Number of consecutive signal samples required to consider the signal stable
    static const uint8_t n_stable_samples = 5;
    // The pin is normally low and goes high N times to encode the digit N
    static bool debounced_pin = false;
    bool hardware_pin = (PIND & (1 << PIND3));

    if (debounced_pin == hardware_pin) {
        counter = 0;
    } else if (++counter == n_stable_samples) {
        debounced_pin = hardware_pin;
        counter = 0;
    } else {
        // Do nothing
    }

    return debounced_pin;
}
