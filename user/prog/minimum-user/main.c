#include <stdint.h>
#include "minemu/user_abi.h"

#define LINE_MAX_LEN 25U

static void write_text(const char *text, uint32_t length) {
    (void)ioctl(MINEMU_FD_UART_OUT, MINEMU_IOCTL_UART_WRITE,
                (uintptr_t)text, length);
}

static void write_str(const char *text) {
    uint32_t length = 0;
    while (text[length] != '\0') {
        length++;
    }
    write_text(text, length);
}

static int read_char(char *c) {
    return (int)ioctl(MINEMU_FD_UART_IN, MINEMU_IOCTL_UART_READ,
                      (uintptr_t)c, 1U);
}

void minemu_user_main(void) {
    for (;;) {
        char line[LINE_MAX_LEN + 1U];
        uint32_t length = 0;

        write_str("msh> ");

        for (;;) {
            char c;

            if (read_char(&c) != 1) {
                continue;
            }

            if (c == '\n') {
                break;
            }

            if (c == '\b' || c == 0x7f) {
                if (length > 0) {
                    length--;
                }
                continue;
            }

            if (c == ' ') {
                if (length == 0) {
                    continue;
                }
                if (length <= LINE_MAX_LEN && line[length - 1U] == ' ') {
                    continue;
                }
            }

            if (length < LINE_MAX_LEN) {
                line[length] = c;
            }
            length++;
        }

        if (length == 0 || length > LINE_MAX_LEN) {
            continue;
        }

        if (line[length - 1U] == ' ') {
            length--;
        }
        line[length] = '\0';

        uint32_t word_len = 0;
        while (word_len < length && line[word_len] != ' ') {
            word_len++;
        }

        if (word_len == 4U && line[0] == 'e' && line[1] == 'c' &&
            line[2] == 'h' && line[3] == 'o') {
            if (word_len < length) {
                write_text(&line[word_len + 1U], length - word_len - 1U);
            }
            write_str("\n");
        } else {
            write_str("command not found: ");
            write_text(line, word_len);
            write_str("\n");
        }
    }
}