.set noreorder
.global __start

__start:
    # default graphic memory base address
    lui $t0, 0x0500
    li $t1, 0xffff
loop:
    sw $t0, ($t0)
    andi $t2, $t0, 0xffff
    beq $t1, $t2, halt
    nop
    b loop
    addiu $t0, $t0, 1

halt:
    b halt
    nop
