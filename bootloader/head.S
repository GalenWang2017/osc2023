#include "mm.h"

.section ".text.boot"

.globl _start
_start:
    mrs	x0, mpidr_el1		
	and	x0, x0,#0xFF		// Check processor id
	cbnz	x0, proc_hang		// Hang for all non-primary CPU

    adr x1, _bootloader
    subs x0, lr, x1
    b.ne relocate

    ldr x0, =_stack_top
    mov sp, x0

    adr x0, bss_begin
    adr x1, bss_end
    sub x1, x1, x0
    bl memzero
    bl bootloader

relocate:
    ldr x1, =_kernel
    ldr x2, =_bootloader
    ldr w3, =_loader_size
1:
    ldr x4, [x1], #8
    str x4, [x2], #8
    subs w3, w3, #1
    cbnz w3, 1b

    ldr x30, =_bootloader
    br x30
    b 1b

proc_hang: 
	b 	proc_hang



