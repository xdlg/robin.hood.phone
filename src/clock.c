#include "clock.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

static volatile uint32_t now_ms;

void clock_init()
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    TIMSK1 = 0;
    OCR1A = 0;

    // CTC mode
    TCCR1B |= (1 << WGM12);
    // Prescaler 8
    TCCR1B |= (1 << CS11);
    // 1000 Hz (10000000 Hz / ((1249 + 1) * 8))
    OCR1A = 1249;
    // Enable interrupt
    TIMSK1 |= (1 << OCIE1A);
}

uint32_t clock_now_ms()
{
    uint32_t now;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        now = now_ms;
    }
    return now;
}

ISR(TIMER1_COMPA_vect)
{
    now_ms++;
}
