#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "AVR-UART-lib/usart.h"

#define BAUD_RATE 9600

int main(void)
{
    uart_init(BAUD_CALC(BAUD_RATE));
    sei();

    // Set PB2 as output
    DDRB |= (1 << DDB2);

    _delay_ms(500);
    uart_puts("PDIAL    OGG\n");

    while (1) {
        // Toggle PB2
        _delay_ms(1000);
        PINB |= (1 << PINB2);
    }
}
