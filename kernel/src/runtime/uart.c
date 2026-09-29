#include "minemu/uart.h"
#include "minemu/platform.h"

void minemu_uart_putc(char c){
	while ((MINEMU_UART0->status & MINEMU_UART_STATUS_TX_READY) == 0){
	}

	MINEMU_UART0->tx_data = (uint32_t)c;
}

void minemu_uart_puts(const char *str){
	while(*str != '\0'){
		minemu_uart_putc(*str);
		str++;
	}
}
