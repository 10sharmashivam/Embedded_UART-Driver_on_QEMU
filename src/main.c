#include "uart.h"

/* System clock frequency */
#define SYSTEM_CLOCK 16000000

/* Test message */
const char* test_message = "UART Driver Test\r\n";

int main(void) {
    // Initialize UART with 115200 baud rate
    uart_init(115200);
    
    // Send test message
    uart_send_string(test_message);
    
    // Enable UART interrupts
    uart_enable_interrupts();
    
    // Main loop
    while (1) {
        // Echo any received characters
        char c = uart_get_char();
        uart_send_char(c);
    }
    
    return 0;
} 