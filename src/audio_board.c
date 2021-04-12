#include "audio_board.h"
#include "AVR-UART-lib/usart.h"
#include "timer.h"

#include <avr/io.h>
#include <string.h>

static bool audio_board_play_file(const char* file_name);

void audio_board_init()
{
    // Set PD2 as input (connected to the ACT pin of the audio board)
    DDRD &= ~(1 << PIND2);

    uart_init(BAUD_CALC(BAUD_RATE));
}

bool audio_board_has_started()
{
    char buffer[RX_BUFFER_SIZE];

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

bool audio_board_is_playing()
{
    // The pin goes low when an audio file is playing
    return !(PIND & (1 << PIND2));
}

bool audio_board_play_dial_tone()
{
    return audio_board_play_file("DIAL");
}

bool audio_board_play_busy_tone()
{
    return audio_board_play_file("BUSY");
}

static bool audio_board_play_file(const char* file_name)
{
    const uint8_t max_file_name_length = 8;
    if (strlen(file_name) > max_file_name_length) {
        return false;
    }

    // The spaces are placeholders for the file name
    char request[] = "P        OGG\n";
    strncpy(&request[1], file_name, strlen(file_name));
    uart_puts(request);

    const uint32_t timeout_ms = 1000;
    timer_start(timeout_ms);

    while (!timer_has_timed_out() && !audio_board_is_playing()) {}

    return !timer_has_timed_out();
}
