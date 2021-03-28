src = src
main = main
build = build
part = m168p
mcu = atmega168p
avrFrequency = 10000000
programmer = atmelice_isp
cflags = -Wall -Werror -Wextra -Os -DF_CPU=$(avrFrequency)

help:
	@echo 'help	Shows this text.'
	@echo 'check	Checks that the programmer is connected and can communicate with the target.'
	@echo 'erase	Erases the target.'
	@echo 'clean	Cleans build files.'
	@echo 'all	Compiles program to HEX file'
	@echo 'flash	Write compiled HEX file to target flash.'
	@echo 'program Compiles and writes program to target flash'

check:
	avrdude -p $(part) -c $(programmer) -n

erase:
	avrdude -p $(part) -c $(programmer) -e

clean:
	rm -rf $(build)

all:
	mkdir -p $(build)
	avr-gcc $(cflags) -mmcu=$(mcu) -o $(build)/$(main).o $(src)/$(main).c
	avr-objcopy -j .text -j .data -O ihex $(build)/$(main).o $(build)/$(main).hex

flash:
	avrdude -p $(part) -c $(programmer) -U flash:w:$(build)/$(main).hex:i

program: all flash
