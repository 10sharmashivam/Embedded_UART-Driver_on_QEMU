#include "uart.h"

/* Helper function to write to a register */
static inline void write_reg(uint32_t reg, uint32_t value) {
    *(volatile uint32_t*)reg = value;
}

/* Helper function to read from a register */
static inline uint32_t read_reg(uint32_t reg) {
    return *(volatile uint32_t*)reg;
}

/* Initialize UART with specified baud rate */
void uart_init(uint32_t baud_rate) {
    // Disable UART
    write_reg(UART_CTL, 0);
    
    // Clear pending interrupts
    write_reg(UART_ICR, 0x7FF);
    
    // Set baud rate (assuming 16MHz system clock)
    uint32_t baud_div = 16000000 / (16 * baud_rate);
    write_reg(UART_IBRD, baud_div >> 6);
    write_reg(UART_FBRD, baud_div & 0x3F);
    
    // Set line control (8N1)
    write_reg(UART_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
    
    // Enable UART, TX, and RX
    write_reg(UART_CTL, UART_CTL_UARTEN | UART_CTL_TXE | UART_CTL_RXE);
}

/* Send a single character */
void uart_send_char(char c) {
    // Wait until transmit FIFO is not full
    while (read_reg(UART_FR) & UART_FR_TXFF);
    
    // Write character to data register
    write_reg(UART_DR, c);
}

/* Receive a single character */
char uart_get_char(void) {
    // Wait until receive FIFO is not empty
    while (read_reg(UART_FR) & UART_FR_RXFE);
    
    // Read and return character from data register
    return (char)(read_reg(UART_DR) & 0xFF);
}

/* Send a null-terminated string */
void uart_send_string(const char* str) {
    while (*str) {
        uart_send_char(*str++);
    }
}

/* Enable UART interrupts */
void uart_enable_interrupts(void) {
    // Enable receive interrupt
    write_reg(UART_IM, 1 << 4);  // RXIM
}

/* Disable UART interrupts */
void uart_disable_interrupts(void) {
    write_reg(UART_IM, 0);
}

/* UART Interrupt Handler */
void uart_irq_handler(void) {
    // Check if receive interrupt is pending
    if (read_reg(UART_MIS) & (1 << 4)) {
        // Read received character
        char c = uart_get_char();
        
        // Echo back the received character
        uart_send_char(c);
        
        // Clear the interrupt
        write_reg(UART_ICR, 1 << 4);
    }
} 