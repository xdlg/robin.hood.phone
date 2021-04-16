#include "audio_board.h"
#include "AVR-UART-lib/usart.h"
#include "clock.h"

#include <avr/io.h>
#include <string.h>

void audio_board_init()
{
    // Set PB5 as input (connected to the ACT pin of the audio board)
    DDRB &= ~(1 << DDB5);

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
    return !(PINB & (1 << PINB5));
}

bool audio_board_play_dial_tone()
{
    return audio_board_play_file("DIAL");
}

bool audio_board_play_busy_tone()
{
    return audio_board_play_file("BUSY");
}

bool audio_board_play_file(const char* file_name)
{
    const uint8_t max_file_name_length = 8;
    if (strlen(file_name) > max_file_name_length) {
        return false;
    }

    // The spaces are placeholders for the file name
    char request[] = "P        OGG\n";
    strncpy(&request[1], file_name, strlen(file_name));
    uart_puts(request);

    const uint32_t timeout_ms = 500;
    uint32_t then_ms = clock_now_ms();

    while (!audio_board_is_playing()) {
        if ((clock_now_ms() - then_ms) >= timeout_ms) {
            return false;
        }
    }

    return true;
}

bool audio_board_stop_playback()
{
    char request[] = "q\n";
    uart_puts(request);

    const uint32_t timeout_ms = 500;
    uint32_t then_ms = clock_now_ms();

    while (audio_board_is_playing()) {
        if ((clock_now_ms() - then_ms) >= timeout_ms) {
            return false;
        }
    }

    return true;
}
