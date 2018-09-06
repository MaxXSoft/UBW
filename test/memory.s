.set noreorder
.global __start

__start:
    # get seed
    jal seed
    nop
    # get base address
    lui $t8, 0xa000
    andi $t0, $v0, 0xfff0
    add $t8, $t8, $t0


    # get random number 0
    jal get_rand
    move $a0, $v0
    # store random number 0
    move $s0, $v0
    sw $s0, 0x00($t8)

    # get random number 1
    jal get_rand
    move $a0, $v0
    # store random number 1
    move $s1, $v0
    sw $s1, 0x04($t8)

    # get random number 2
    jal get_rand
    move $a0, $v0
    # store random number 2
    move $s2, $v0
    sw $s2, 0x08($t8)

    # get random number 3
    jal get_rand
    move $a0, $v0
    # store random number 3
    move $s3, $v0
    sw $s3, 0x0c($t8)

    # get random number 4
    jal get_rand
    move $a0, $v0
    # store random number 4
    move $s4, $v0
    sw $s4, 0x10($t8)

    # get random number 5
    jal get_rand
    move $a0, $v0
    # store random number 5
    move $s5, $v0
    sw $s5, 0x14($t8)

    # get random number 6
    jal get_rand
    move $a0, $v0
    # store random number 6
    move $s6, $v0
    sw $s6, 0x18($t8)

    # get random number 7
    jal get_rand
    move $a0, $v0
    # store random number 7
    move $s7, $v0
    sw $s7, 0x1c($t8)


    # validate memory 0
    lw $t0, 0x00($t8)
    bne $t0, $s0, error
    nop

    # validate memory 1
    lw $t0, 0x04($t8)
    bne $t0, $s1, error
    nop

    # validate memory 2
    lw $t0, 0x08($t8)
    bne $t0, $s2, error
    nop

    # validate memory 3
    lw $t0, 0x0c($t8)
    bne $t0, $s3, error
    nop

    # validate memory 4
    lw $t0, 0x10($t8)
    bne $t0, $s4, error
    nop

    # validate memory 5
    lw $t0, 0x14($t8)
    bne $t0, $s5, error
    nop

    # validate memory 6
    lw $t0, 0x18($t8)
    bne $t0, $s6, error
    nop

    # validate memory 7
    lw $t0, 0x1c($t8)
    bne $t0, $s7, error
    nop


success:
    # green light
    li $t0, 2
    lui $t1, 0xbfd0
    sw $t0, 0x5008($t1)
    sw $t0, 0x500c($t1)
    # halt the CPU
    b halt
    nop

error:
    # red light
    li $t0, 1
    lui $t1, 0xbfd0
    sw $t0, 0x5008($t1)
    sw $t0, 0x500c($t1)
    # halt the CPU
    b halt
    nop

halt:
    b halt
    nop

seed:
    lui $t0, 0xbfd0
    jr $ra
    lw $v0, 0x5018($t0)

get_rand:
    li $v0, 0x343fd
    mult $a0, $v0
    mflo $v0
    li $t1, 0x269ec3
    add $v0, $v0, $t1
    srl $v0, $v0, 0x10
    jr $ra
    andi $v0, $v0, 0xffff
