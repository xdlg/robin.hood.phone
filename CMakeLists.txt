cmake_minimum_required(VERSION 3.17)
project("robinhood.phone")

set(F_CPU 10000000UL)
set(MCU atmega168p)
set(PART m168p)
set(PROGRAMMER atmelice_isp)
set(BAUD_RATE 9600)
set(TX_BUFFER_SIZE 16)
set(RX_BUFFER_SIZE 128)

find_program(AVR_CC avr-gcc)
find_program(AVR_OBJCOPY avr-objcopy)
find_program(AVR_DUDE avrdude)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_C_COMPILER ${AVR_CC})

add_definitions(
    -DF_CPU=${F_CPU}
    -DBAUD_RATE=${BAUD_RATE}
    -DTX_BUFFER_SIZE=${TX_BUFFER_SIZE}
    -DRX_BUFFER_SIZE=${RX_BUFFER_SIZE}
)

set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})

set(AVR-UART-lib AVR-UART-lib)
add_library(${AVR-UART-lib} ${AVR-UART-lib}/usart.c)
# No warnings for this library
target_compile_options(
    ${AVR-UART-lib} PRIVATE
    -mmcu=${MCU}
    -Os
)

add_compile_options(
    -mmcu=${MCU}
    -Os
    -Wall
    -Wextra
    -Werror
)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(${PROJECT_NAME}
    src/main.c
    src/audio_board.c
    src/clock.c
    src/debug_led.c
    # src/dial.c
)

target_link_libraries(${PROJECT_NAME}
    ${AVR-UART-lib}
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.elf)
add_custom_target(hex ${AVR_OBJCOPY} -j .text -j .data -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex DEPENDS ${PROJECT_NAME})
add_custom_target(flash ${AVR_DUDE} -p ${PART} -c ${PROGRAMMER} -U flash:w:${PROJECT_NAME}.hex:i DEPENDS hex)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_NAME}.hex")
