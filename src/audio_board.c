#include "audio_board.h"
#include "AVR-UART-lib/usart.h"

#include <string.h>

#define PLAYBACK_DONE_RESPONSE  "done"
#define NO_FILE_RESPONSE        "NoFile"
#define PLAY_DIAL_TONE_REQUEST  "PDIAL    OGG\n"
#define PLAY_DIAL_TONE_RESPONSE "DIAL    OGG"
#define PLAY_BUSY_TONE_REQUEST  "PBUSY    OGG\n"
#define PLAY_BUSY_TONE_RESPONSE "BUSY    OGG"

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

bool audio_board_is_done_playing()
{
    /// @todo Add timeout
    while (true) {
        if (uart_AvailableBytes() > 0) {
            uart_getln(buffer, RX_BUFFER_SIZE);
            if (strstr(buffer, PLAYBACK_DONE_RESPONSE)) {
                return true;
            }
        }
    }

    return false;
}

bool audio_board_play_dial_tone()
{
    uart_puts(PLAY_DIAL_TONE_REQUEST);

    /// @todo Add timeout
    while (true) {
        if (uart_AvailableBytes() > 0) {
            uart_getln(buffer, RX_BUFFER_SIZE);
            if (strstr(buffer, PLAY_DIAL_TONE_RESPONSE)) {
                return true;
            } else if (strstr(buffer, NO_FILE_RESPONSE)) {
                return false;
            } else {
                // Do nothing
            }
        }
    }

    return false;
}

bool audio_board_play_busy_tone()
{
    uart_puts(PLAY_BUSY_TONE_REQUEST);

    /// @todo Add timeout
    while (true) {
        if (uart_AvailableBytes() > 0) {
            uart_getln(buffer, RX_BUFFER_SIZE);
            if (strstr(buffer, PLAY_BUSY_TONE_RESPONSE)) {
                return true;
            } else if (strstr(buffer, NO_FILE_RESPONSE)) {
                return false;
            } else {
                // Do nothing
            }
        }
    }

    return false;
}
