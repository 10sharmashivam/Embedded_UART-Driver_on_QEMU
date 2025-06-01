#include "uart.h"

/* System clock frequency */
#define SYSTEM_CLOCK 16000000

/* Test message */
const char* test_message = "UART Driver Test\r\n";

/* Error handling function */
void handle_uart_error(uart_error_t error, const char* operation) {
    if (error != UART_OK) {
        // In a real system, you might want to log this or handle it differently
        const char* error_str = uart_error_to_string(error);
        // TODO: Implement proper error logging
    }
}

int main(void) {
    uart_error_t error;
    char received_char;
    
    // Initialize UART with 115200 baud rate
    error = uart_init(115200);
    handle_uart_error(error, "UART initialization");
    
    // Clear any pending errors
    uart_clear_errors();
    
    // Send test message
    error = uart_send_string(test_message, UART_TIMEOUT_MS);
    handle_uart_error(error, "Sending test message");
    
    // Enable UART interrupts
    error = uart_enable_interrupts();
    handle_uart_error(error, "Enabling interrupts");
    
    // Main loop
    while (1) {
        // Echo any received characters with error handling
        error = uart_get_char(&received_char, UART_TIMEOUT_MS);
        if (error == UART_OK) {
            error = uart_send_char(received_char, UART_TIMEOUT_MS);
            handle_uart_error(error, "Echoing character");
        } else if (error != UART_ERROR_TIMEOUT) {
            // Only handle non-timeout errors
            handle_uart_error(error, "Receiving character");
            
            // Clear errors and continue
            uart_clear_errors();
        }
        
        // Check for any pending errors
        error = uart_get_error_status();
        if (error != UART_OK) {
            handle_uart_error(error, "Checking error status");
            uart_clear_errors();
        }
    }
    
    return 0;
} 