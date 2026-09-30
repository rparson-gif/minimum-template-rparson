#ifndef MINEMU_UART_H
#define MINEMU_UART_H

void minemu_uart_putc(char c);
void minemu_uart_puts(const char *str);
void minemu_uart_irq_handler(void);
int minemu_uart_getc(char *c);

#endif
