# robinhood.phone

The robinhood phone is an old rotary phone that plays audio files associated with a predefined phone
number. See [Hackaday page](https://hackaday.io/project/178458-robinhoodphone) for details.

## Hardware

- [ATMega168PA](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-9223-Automotive-Microcontrollers-ATmega48PA-ATmega88PA-ATmega168PA_Datasheet.pdf) on a [Crumb168 V2.3](https://www.terraelectronica.ru/pdf/show?pdf_file=%252Fz%252FDatasheet%252FA%252FAVR-CRUMB168-USB.pdf) by Chip45
- [Adafruit Audio FX Mini Sound Board](https://www.adafruit.com/product/2341)
- [Adafruit LiIon/LiPoly Backpack Add-On](https://www.adafruit.com/product/2124)

## Software

On power up, the software on the ATMega runs the following sequence:
1. Initializes the hardware, giving the audio board some time to start.
2. Requests the audio board to start playing the dial tone, which is an audio file of finite length.
3. Polls the dial for activity while the dial tone is playing. If nothing is dialed until the dial tone has finished playing, the phone goes straight to step 7.
4. Decodes each new dialed digit, waiting a few seconds between each digit. If no new digit is input within a set time, or if the maximum number of digits have been dialed, the phone tries to compose that number.
5. Requests the audio board to play the audio file corresponding to the dialed number. If this file doesn't exist, the phone goes straight to step 7.
6. Waits a few seconds after the file has finished playing.
7. Requests the audio board to start playing the busy tone, which is an audio file of finite length.
8. Stays idle once the busy tone has finished playing.

## Required packages

```bash
sudo apt-get install avr-libc avrdude gcc-avr
```
