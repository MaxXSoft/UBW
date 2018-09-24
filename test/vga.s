.set noreorder
.global __start

__start:
    # default graphic memory base address
    lui $t0, 0x0500
    li $t1, 0x06000000
loop:
    sw $t0, ($t0)
    beq $t1, $t0, halt
    nop
    b loop
    addiu $t0, $t0, 4

halt:
    b halt
    nop
