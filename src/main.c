#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>

#include "AVR-UART-lib/usart.h"

bool init();

int main()
{
    bool success = init();
    // uart_puts("PDIAL    OGG\n");

    if (success) {
        PINB |= (1 << PINB2);
    }

    while (1) {
        if (!success) {
            // Toggle PB2
            _delay_ms(1000);
            PINB |= (1 << PINB2);
        }
    }
}

bool init()
{
    // Set PB2 as output
    DDRB |= (1 << DDB2);

    uart_init(BAUD_CALC(BAUD_RATE));
    sei();

    char buffer[RX_BUFFER_SIZE];

    /// @todo Add timeout
    while (true) {
        uart_getln(buffer, RX_BUFFER_SIZE);
        // We use strstr instead of strcmp because there might be some carriage returns or line
        // feeds left at the beginning or at the end of the string. Either the sound board or the
        // UART library seem to be a little erratic when it comes to those characters.
        if (strstr(buffer, "Adafruit FX Sound Board")) {
            // Good enough for us, the board is communicating
            return true;
        }
    }

    return false;
}
