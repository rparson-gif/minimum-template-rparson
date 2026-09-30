#include "minemu/boot.h"
#include "minemu/trap.h"
#include "minemu/trace.h"
#include "minemu/uart.h"
#include "minemu/irq.h"
#include "minemu/platform.h"

#define LINE_MAX_LEN 20

void minemu_kernel_main(const struct minemu_boot_info *boot_info) {
    if ((uintptr_t)boot_info != MINEMU_BOOT_INFO_VADDR ||
        boot_info->magic != MINEMU_BOOT_INFO_MAGIC ||
        boot_info->version != MINEMU_ABI_VERSION ||
        boot_info->size != sizeof(*boot_info) ||
        boot_info->system_rom_base != UINT32_C(0x08000000) ||
        boot_info->direct_map_vaddr != UINT32_C(0xc0000000) ||
        boot_info->direct_map_paddr != UINT32_C(0x40000000) ||
        boot_info->direct_map_size != UINT32_C(0x04000000)) {
        minemu_trace_event(UINT32_C(0xb007bad0));
        minemu_fail_stop();
    }

    MINEMU_INTERRUPT->priority_systick = MINEMU_IRQ_PRIORITY_SYSTICK_RESET;
    MINEMU_INTERRUPT->priority_uart0 = MINEMU_IRQ_PRIORITY_UART0_RESET;
    MINEMU_INTERRUPT->priority_uart1 = MINEMU_IRQ_PRIORITY_UART1_RESET;
    MINEMU_INTERRUPT->priority_block = MINEMU_IRQ_PRIORITY_BLOCK_RESET;


    MINEMU_INTERRUPT->enable = MINEMU_INTERRUPT->enable | (UINT32_C(1) << MINEMU_IRQ_UART0);

    MINEMU_UART0->control = MINEMU_UART_CONTROL_RX_IRQ_ENABLE;

    minemu_irq_enable();

    char line[LINE_MAX_LEN];
    int len = 0;

    minemu_uart_puts("hello world\n");
    
    minemu_uart_puts("msh> ");

    for (;;) {
        char ch;
        if (!minemu_uart_getc(&ch)) {
            continue;
        }
        int c = (unsigned char)ch;

        if (c == 0x08 || c == 0x7f) {
            /* backspace: ignored on an empty line */
            if (len > 0) {
                len--;
            }
        } else if (c == '\n') {
            int i = 0;
            while (i < len && line[i] == ' ') {
                i++;
            }
            if (i < len) {                      
                int s = i;
                while (i < len && line[i] != ' ') {
                    i++;
                }
                int n = i - s;

                if (n == 4 && line[s] == 'e' && line[s + 1] == 'c' &&
                    line[s + 2] == 'h' && line[s + 3] == 'o') {
                    while (i < len && line[i] == ' ') {
                        i++;
                    }
                    while (i < len) {
                        minemu_uart_putc(line[i++]);
                    }
                    minemu_uart_putc('\n');
                } else {
                    minemu_uart_puts("command not found: ");
                    for (int k = 0; k < n; k++) {
                        minemu_uart_putc(line[s + k]);
                    }
                    minemu_uart_putc('\n');
                }
            }
            len = 0;
            minemu_uart_puts("msh> ");
        } else if (len < LINE_MAX_LEN) {
            line[len++] = (char)c;
        }
        
    }
}

