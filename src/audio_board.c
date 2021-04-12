#include "audio_board.h"
#include "AVR-UART-lib/usart.h"

#include <string.h>

static char buffer[RX_BUFFER_SIZE];

void audio_board_init()
{
    uart_init(BAUD_CALC(BAUD_RATE));
}

bool audio_board_has_started()
{
    if (uart_AvailableBytes() > 0) {
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
