# Embedded UART Driver on QEMU (Baremetal ARM Cortex-M)

# Embedded UART Driver for ARM Cortex-M

This project implements a bare-metal UART driver for ARM Cortex-M using QEMU and arm-none-eabi-gcc. The driver provides low-level UART communication capabilities including data transmission, loopback testing, and interrupt handling.

## Features
- Bare-metal UART driver implementation for ARM Cortex-M
- 3-stage register-level firmware logic
- UART data transmission and reception
- Loopback testing capability
- Interrupt handling
- QEMU-based testing environment
- GRUB bootloader integration
- Ubuntu boot support

## Prerequisites
- arm-none-eabi-gcc toolchain
- QEMU with ARM support
- GRUB tools
- Make

## Building the Project
```bash
make
```

## Running the Project
There are two ways to run the project:

1. Direct UART driver execution:
```bash
make run
```

2. Using GRUB bootloader:
```bash
make run_grub
```

## Project Structure
- `src/` - Source files
  - `main.c` - Main application code
  - `uart.c` - UART driver implementation
  - `uart.h` - UART driver header
- `startup/` - Startup code and linker scripts
- `grub.cfg` - GRUB configuration
- `Makefile` - Build configuration

## Implementation Details
1. Bare-metal UART Driver
   - Register-level hardware access
   - Interrupt-driven reception
   - Polling-based transmission
   - Configurable baud rate

2. 3-Stage Firmware Logic
   - Initialization stage (baud rate, data format)
   - Data transmission stage (send/receive)
   - Interrupt handling stage (IRQ management)

3. GRUB Integration
   - Multiboot support
   - Ubuntu boot capability
   - Custom boot menu

## License
MIT License 