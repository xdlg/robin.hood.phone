#include "timer.h"

#include <avr/io.h>
#include <util/atomic.h>

static volatile uint32_t t_ms;
static volatile bool has_timed_out;
static uint32_t max_t_ms;

// Resets the timer to its default configuration
static void timer_reset();

void timer_start(uint32_t timeout_ms)
{
    timer_reset();
    t_ms = 0;
    has_timed_out = false;
    max_t_ms = timeout_ms;

    // CTC mode
    TCCR1B |= (1 << WGM12);
    // Prescaler 8
    TCCR1B |= (1 << CS11);
    // 1000 Hz (10000000 Hz / ((1249 + 1) * 8))
    OCR1A = 1249;
    // Enable interrupt
    TIMSK1 |= (1 << OCIE1A);
}

bool timer_has_timed_out()
{
    bool done;

    // Make sure that the flag doesn't change while reading
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        done = has_timed_out;
    }

    return done;
}

static void timer_reset()
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    TIMSK1 = 0;
    OCR1A = 0;
}

ISR(TIMER1_COMPA_vect)
{
    t_ms++;

    if (t_ms >= max_t_ms) {
        has_timed_out = true;
        timer_reset();
    }
}
