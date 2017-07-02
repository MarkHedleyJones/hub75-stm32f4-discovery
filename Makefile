# put your *.o targets here, make should handle the rest!

SRCS = main.c stm32f4xx_it.c system_stm32f4xx.c

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)

PROJ_NAME=main
OUTPUT_DIR=bin

# that's it, no need to change anything below this line!

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS  = -g -O2 -Wall -Tstm32_flash.ld
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

###################################################

vpath %.c src
vpath %.a lib

ROOT=$(shell pwd)

CFLAGS += -Iinc -Ilib -Ilib/inc
CFLAGS += -Ilib/inc/core -Ilib/inc/peripherals

SRCS += lib/startup_stm32f4xx.s # add startup file to build

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: $(OUTPUT_DIR) lib proj

all: $(OUTPUT_DIR) lib proj

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

lib:
	$(MAKE) -C lib

proj: 	$(OUTPUT_DIR)/$(PROJ_NAME).elf

$(OUTPUT_DIR)/$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -Llib -lstm32f4
	$(OBJCOPY) -O ihex $(OUTPUT_DIR)/$(PROJ_NAME).elf $(OUTPUT_DIR)/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(OUTPUT_DIR)/$(PROJ_NAME).elf $(OUTPUT_DIR)/$(PROJ_NAME).bin

clean:
	$(MAKE) -C lib clean
	rm -f $(OUTPUT_DIR)/$(PROJ_NAME).elf
	rm -f $(OUTPUT_DIR)/$(PROJ_NAME).hex
	rm -f $(OUTPUT_DIR)/$(PROJ_NAME).bin

flash: proj
	st-flash write $(OUTPUT_DIR)/$(PROJ_NAME).bin 0x8000000
