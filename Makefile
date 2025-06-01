# Toolchain
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)as
LD = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump

# Flags
CFLAGS = -mcpu=cortex-m3 -mthumb -Wall -g -O0
LDFLAGS = -T linker.ld -nostartfiles

# Source files
SRCS = src/main.c src/uart.c
OBJS = $(SRCS:.c=.o)

# Target
TARGET = uart_driver

# GRUB and Ubuntu image
GRUB_CFG = grub.cfg
UBUNTU_IMG = ubuntu.img
GRUB_IMG = grub.img

all: $(TARGET).elf $(TARGET).bin $(GRUB_IMG)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(GRUB_IMG): $(TARGET).bin $(GRUB_CFG)
	# Create GRUB image with UART driver
	grub-mkrescue -o $@ .

run: $(GRUB_IMG)
	qemu-system-arm -M lm3s6965evb -kernel $(TARGET).bin -nographic

run_grub: $(GRUB_IMG)
	qemu-system-arm -M lm3s6965evb -drive file=$(GRUB_IMG),format=raw -nographic

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin $(GRUB_IMG)

.PHONY: all clean run run_grub 