.set noreorder
.global __start
.extern KernelMain

.org 0x000
__start:
    # enable interrupt
    mfc0 $t0, $12
    ori $t0, $t0, 1
    mtc0 $t0, $12
    # initialize stack
    la $sp, _stack
    # initialize global pointer
    la $gp, _gp
    # initialize kernel
    la $t0, KernelMain
    li $t1, 0x20000000
    subu $t0, $t0, $t1   # kseg1 -> kseg0
    jr $t0
    nop

.org 0x380
_interrupt_entry:
    eret
    nop
