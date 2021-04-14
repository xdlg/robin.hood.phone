#include "dial.h"
#include "clock.h"

#include <avr/io.h>

#define MAX_DIGITS_IN_NUMBER 8
#define INVALID_TIME 0xFFFF
#define INVALID_DIGIT 0xFF

// Dialled number (the last char is a null terminator)
static char number[MAX_DIGITS_IN_NUMBER + 1];
// Index of the next digit to store in the number string
static uint8_t digit_index;

// Stores a digit in the number string
static void dial_store_digit(uint8_t digit);

// Polls the dialling pin, which indicates that a digit dialling has started.
// Must be called at regular intervals.
static bool dial_poll_dialling_pin();

// Polls the digit pin, which indicates the digit being dialled.
// Must be called at regular intervals.
static bool dial_poll_digit_pin();

// Resets the dialled number
static void dial_reset_number();

// Converts a digit to ASCII.
// Returns 0 if not representable on a single digit.
static char dial_digit_to_ascii(uint8_t digit);

void dial_init()
{
    dial_reset_number();
    digit_index = 0;

    // Set PD2/INT0 as input with pull-up (connected to the "dialling in progress" pin)
    DDRD &= ~(1 << DDD2);
    PORTD |= (1 << PORTD2);

    // Set PD3/INT1 as input with pull-up (connected to the "number" pin)
    DDRD &= ~(1 << DDD3);
    PORTD |= (1 << PORTD3);
}

dial_state_t dial_state()
{
    if (digit_index >= MAX_DIGITS_IN_NUMBER) {
        // We're already at capacity, get out of here
        return DIAL_STATE_FINISHED;
    }

    // If the dial was active, and then showed no activity for some time,
    // we consider the dialling finished.
    static const uint32_t activity_timeout_ms = 3000;
    static uint32_t last_activity_time_ms = INVALID_TIME;
    static uint8_t digit = INVALID_DIGIT;
    if ((last_activity_time_ms != INVALID_TIME)
        && ((clock_now_ms() - last_activity_time_ms) >= activity_timeout_ms)) {
        // Store the last valid digit
        if (digit != INVALID_DIGIT) {
            dial_store_digit(digit);
        }

        return DIAL_STATE_FINISHED;
    }

    static dial_state_t state = DIAL_STATE_WAITING;
    static bool dialling_pin = true;
    bool new_dialling_pin = dial_poll_dialling_pin();

    if (dialling_pin != new_dialling_pin) {
        last_activity_time_ms = clock_now_ms();
        state = DIAL_STATE_DIALLING;
        dialling_pin = new_dialling_pin;

        // A falling edge on the dialling pin means that dialling has started
        if (!dialling_pin) {
            // Store the last valid digit
            if (digit != INVALID_DIGIT) {
                dial_store_digit(digit);
            }
            // Start counting edges on the digit pin
            digit = 0;
        }
    }

    static bool digit_pin = false;
    bool new_digit_pin = dial_poll_digit_pin();

    if (digit_pin != new_digit_pin) {
        last_activity_time_ms = clock_now_ms();
        state = DIAL_STATE_DIALLING;
        digit_pin = new_digit_pin;

        // A rising edge on the digit pin encodes a digit increment
        if (digit_pin) {
            digit++;
        }
    }

    return state;
}

char* dial_number()
{
    return number;
}

static void dial_store_digit(uint8_t digit)
{
    if (digit == 10) {
        // 0 is encoded with 10 rising edges
        digit = 0;
    }
    number[digit_index++] = dial_digit_to_ascii(digit);
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

static void dial_reset_number()
{
    for (uint8_t i = 0; i < sizeof(number); i++) {
        number[i] = '\0';
    }
}

static char dial_digit_to_ascii(uint8_t digit)
{
    return (digit < 10) ? ('0' + digit) : '\0';
}
