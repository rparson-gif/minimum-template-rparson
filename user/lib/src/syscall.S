.syntax unified
.cpu cortex-a9
.arm

.global ioctl
.type ioctl, %function

ioctl:
    push {r7, lr}
    mov r7, #1
    svc #0
    pop {r7, pc}
