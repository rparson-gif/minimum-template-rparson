#include "minemu/uart.h"
#include "minemu/platform.h"
#include "minemu/irq.h"

#define MINEMU_UART_RX_BUFFER_SIZE 4096

static char uart_rx_buffer[MINEMU_UART_RX_BUFFER_SIZE];
static unsigned int uart_rx_head = 0;
static unsigned int uart_rx_tail = 0;

void minemu_uart_putc(char c){
	while ((MINEMU_UART0->status & MINEMU_UART_STATUS_TX_READY) == 0){
	}

	MINEMU_UART0->tx_data = (uint32_t)(uint8_t)c;
}

void minemu_uart_puts(const char *str){
	while(*str != '\0'){
		minemu_uart_putc(*str);
		str++;
	}
}

void minemu_uart_irq_handler(void){
	while((MINEMU_UART0->status & MINEMU_UART_STATUS_RX_READY) != 0){
		char c = (char)MINEMU_UART0->rx_data;

		unsigned int next_head = (uart_rx_head + 1) % MINEMU_UART_RX_BUFFER_SIZE;

		if(next_head != uart_rx_tail){
			uart_rx_buffer[uart_rx_head] = c;
			uart_rx_head = next_head;
		}
	}
}


int minemu_uart_getc(char *c){
	int available = 0;
	minemu_irq_disable();

	if(uart_rx_tail != uart_rx_head){
		*c = uart_rx_buffer[uart_rx_tail];

		uart_rx_tail = (uart_rx_tail + 1) % MINEMU_UART_RX_BUFFER_SIZE;

		available = 1;
	}

	minemu_irq_enable();

	return available;
}
