MCU = atmega328p
F_CPU = 16000000UL
BAUD = 9600

CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DBAUD=$(BAUD) -Os -Wall -Wextra
TARGET = main

SRC_DIR = src
INC_DIR = inc
SRCS = $(wildcard $(SRC_DIR)/*.c)

all: $(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	@echo "Build complete! Flash with: make flash"

$(TARGET).elf: $(SRCS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $^
	avr-size --format=avr --mcu=$(MCU) $@

clean:
	rm -f *.elf *.hex *.o

.PHONY: all clean
