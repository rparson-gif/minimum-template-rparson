#ifndef MINEMU_SYSCALL_H
#define MINEMU_SYSCALL_H

#define MINEMU_SYSCALL_IOCTL 1
#define MINEMU_SYSCALL_ERROR (-1)

#ifndef __ASSEMBLER__
#include <stdint.h>
#include "minemu/trap.h"

void minemu_svc_trampoline(void) __attribute__((noreturn));

struct minemu_trap_frame *minemu_svc_dispatch(
    struct minemu_trap_frame *frame);
#endif

#endif
