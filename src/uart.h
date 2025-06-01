#ifndef UART_H
#define UART_H

#include <stdint.h>

/* UART Register Base Address */
#define UART_BASE 0x4000C000

/* UART Register Offsets */
#define UART_DR     (UART_BASE + 0x00)  /* Data Register */
#define UART_RSR    (UART_BASE + 0x04)  /* Receive Status Register */
#define UART_ECR    (UART_BASE + 0x04)  /* Error Clear Register */
#define UART_FR     (UART_BASE + 0x18)  /* Flag Register */
#define UART_ILPR   (UART_BASE + 0x20)  /* IrDA Low-Power Counter Register */
#define UART_IBRD   (UART_BASE + 0x24)  /* Integer Baud Rate Register */
#define UART_FBRD   (UART_BASE + 0x28)  /* Fractional Baud Rate Register */
#define UART_LCRH   (UART_BASE + 0x2C)  /* Line Control Register */
#define UART_CTL    (UART_BASE + 0x30)  /* Control Register */
#define UART_IFLS   (UART_BASE + 0x34)  /* Interrupt FIFO Level Select Register */
#define UART_IM     (UART_BASE + 0x38)  /* Interrupt Mask Register */
#define UART_RIS    (UART_BASE + 0x3C)  /* Raw Interrupt Status Register */
#define UART_MIS    (UART_BASE + 0x40)  /* Masked Interrupt Status Register */
#define UART_ICR    (UART_BASE + 0x44)  /* Interrupt Clear Register */

/* UART Control Register Bits */
#define UART_CTL_UARTEN  (1 << 0)  /* UART Enable */
#define UART_CTL_TXE     (1 << 8)  /* Transmit Enable */
#define UART_CTL_RXE     (1 << 9)  /* Receive Enable */

/* UART Flag Register Bits */
#define UART_FR_TXFF     (1 << 5)  /* Transmit FIFO Full */
#define UART_FR_RXFE     (1 << 4)  /* Receive FIFO Empty */
#define UART_FR_BUSY     (1 << 3)  /* UART Busy */

/* Function Prototypes */
void uart_init(uint32_t baud_rate);
void uart_send_char(char c);
char uart_get_char(void);
void uart_send_string(const char* str);
void uart_enable_interrupts(void);
void uart_disable_interrupts(void);
void uart_irq_handler(void);

#endif /* UART_H */ 