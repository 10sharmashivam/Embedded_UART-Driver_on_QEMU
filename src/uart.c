#include "uart.h"
#include <stddef.h>  

/* Helper function to write to a register */
static inline void write_reg(uint32_t reg, uint32_t value) {
    *(volatile uint32_t*)reg = value;
}

/* Helper function to read from a register */
static inline uint32_t read_reg(uint32_t reg) {
    return *(volatile uint32_t*)reg;
}

/* Convert error code to string */
const char* uart_error_to_string(uart_error_t error) {
    switch (error) {
        case UART_OK:           return "No error";
        case UART_ERROR_INIT:   return "Initialization error";
        case UART_ERROR_TIMEOUT: return "Operation timeout";
        case UART_ERROR_PARAM:  return "Invalid parameter";
        case UART_ERROR_BUSY:   return "UART is busy";
        case UART_ERROR_OVERRUN: return "Buffer overrun";
        case UART_ERROR_FRAMING: return "Framing error";
        case UART_ERROR_PARITY: return "Parity error";
        case UART_ERROR_NOISE:  return "Noise error";
        case UART_ERROR_HARDWARE: return "Hardware error";
        default:               return "Unknown error";
    }
}

/* Initialize UART with specified baud rate */
uart_error_t uart_init(uint32_t baud_rate) {
    if (baud_rate == 0) {
        return UART_ERROR_PARAM;
    }

    // Disable UART
    write_reg(UART_CTL, 0);
    
    // Clear pending interrupts
    write_reg(UART_ICR, 0x7FF);
    
    // Set baud rate (assuming 16MHz system clock)
    uint32_t baud_div = 16000000 / (16 * baud_rate);
    if (baud_div == 0) {
        return UART_ERROR_PARAM;
    }
    
    write_reg(UART_IBRD, baud_div >> 6);
    write_reg(UART_FBRD, baud_div & 0x3F);
    
    // Set line control (8N1)
    write_reg(UART_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
    
    // Enable UART, TX, and RX
    write_reg(UART_CTL, UART_CTL_UARTEN | UART_CTL_TXE | UART_CTL_RXE);
    
    // Verify UART is enabled
    if (!(read_reg(UART_CTL) & UART_CTL_UARTEN)) {
        return UART_ERROR_INIT;
    }
    
    return UART_OK;
}

/* Send a single character with timeout */
uart_error_t uart_send_char(char c, uint32_t timeout_ms) {
    uint32_t start_time = 0; // TODO: Implement system time
    
    // Wait until transmit FIFO is not full
    while (read_reg(UART_FR) & UART_FR_TXFF) {
        // Check for timeout
        if (timeout_ms > 0 && (/* current_time - start_time */ 0 > timeout_ms)) {
            return UART_ERROR_TIMEOUT;
        }
    }
    
    // Check for hardware errors
    uint32_t error_status = read_reg(UART_RSR);
    if (error_status) {
        return UART_ERROR_HARDWARE;
    }
    
    // Write character to data register
    write_reg(UART_DR, c);
    
    return UART_OK;
}

/* Receive a single character with timeout */
uart_error_t uart_get_char(char* c, uint32_t timeout_ms) {
    if (c == NULL) {
        return UART_ERROR_PARAM;
    }
    
    uint32_t start_time = 0; // TODO: Implement system time
    
    // Wait until receive FIFO is not empty
    while (read_reg(UART_FR) & UART_FR_RXFE) {
        // Check for timeout
        if (timeout_ms > 0 && (/* current_time - start_time */ 0 > timeout_ms)) {
            return UART_ERROR_TIMEOUT;
        }
    }
    
    // Check for receive errors
    uint32_t error_status = read_reg(UART_RSR);
    if (error_status & UART_RSR_OE) {
        return UART_ERROR_OVERRUN;
    }
    if (error_status & UART_RSR_FE) {
        return UART_ERROR_FRAMING;
    }
    if (error_status & UART_RSR_PE) {
        return UART_ERROR_PARITY;
    }
    if (error_status & UART_RSR_BE) {
        return UART_ERROR_NOISE;
    }
    
    // Read character from data register
    *c = (char)(read_reg(UART_DR) & 0xFF);
    
    return UART_OK;
}

/* Send a null-terminated string with timeout */
uart_error_t uart_send_string(const char* str, uint32_t timeout_ms) {
    if (str == NULL) {
        return UART_ERROR_PARAM;
    }
    
    while (*str) {
        uart_error_t error = uart_send_char(*str++, timeout_ms);
        if (error != UART_OK) {
            return error;
        }
    }
    
    return UART_OK;
}

/* Enable UART interrupts */
uart_error_t uart_enable_interrupts(void) {
    // Enable receive interrupt
    write_reg(UART_IM, 1 << 4);  // RXIM
    return UART_OK;
}

/* Disable UART interrupts */
uart_error_t uart_disable_interrupts(void) {
    write_reg(UART_IM, 0);
    return UART_OK;
}

/* Clear UART errors */
uart_error_t uart_clear_errors(void) {
    write_reg(UART_ECR, 0xFF);  // Clear all error flags
    return UART_OK;
}

/* Get UART error status */
uart_error_t uart_get_error_status(void) {
    uint32_t error_status = read_reg(UART_RSR);
    
    if (error_status & UART_RSR_OE) {
        return UART_ERROR_OVERRUN;
    }
    if (error_status & UART_RSR_FE) {
        return UART_ERROR_FRAMING;
    }
    if (error_status & UART_RSR_PE) {
        return UART_ERROR_PARITY;
    }
    if (error_status & UART_RSR_BE) {
        return UART_ERROR_NOISE;
    }
    
    return UART_OK;
}

/* UART Interrupt Handler */
void uart_irq_handler(void) {
    // Check if receive interrupt is pending
    if (read_reg(UART_MIS) & (1 << 4)) {
        char received_char;
        uart_error_t error;
        
        // Read received character with timeout
        error = uart_get_char(&received_char, UART_TIMEOUT_MS);
        if (error == UART_OK) {
            // Echo back the received character
            uart_send_char(received_char, UART_TIMEOUT_MS);
        }
        
        // Clear the interrupt
        write_reg(UART_ICR, 1 << 4);
    }
} 