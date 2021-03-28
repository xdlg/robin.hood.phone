#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // Set PB2 as output
    DDRB |= (1 << DDB2);

    while (1) {
        // Toggle PB2
        _delay_ms(1000);
        PINB |= (1 << PINB2);
    }
}
