#ifndef MINEMU_USER_ABI_H
#define MINEMU_USER_ABI_H

#include <stdint.h>

/* Device file descriptors */
#define MINEMU_FD_RNG      UINT32_C(0x10003000)
#define MINEMU_FD_UART_IN  UINT32_C(0x10004000)
#define MINEMU_FD_UART_OUT UINT32_C(0x10004000)
#define MINEMU_FD_TRACE    UINT32_C(0x1000f000)

/* RNG operations */
#define MINEMU_IOCTL_RNG_STATE 0
#define MINEMU_IOCTL_RNG_NEXT  1
#define MINEMU_IOCTL_RNG_SEED  2

/* UART operations */
#define MINEMU_IOCTL_UART_READ  0
#define MINEMU_IOCTL_UART_WRITE 1

/* Trace operations */
#define MINEMU_IOCTL_TRACE_EVENT 0

/* arg0 is required; arg1 is optional for operations such as UART write. */
int32_t ioctl(uint32_t fd, uint32_t operation, ...);

#endif
