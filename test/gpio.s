.set noreorder
.global __start

__start:
    # judge if switch=1
    li $t0, 1
    lui $t1, 0xbfd0
    lw $t1, 0x5000($t1)
    bne $t0, $t1, disp_timer
    nop
disp_keypad:
    # red light
    li $t0, 1
    lui $t1, 0xbfd0
    sw $t0, 0x5008($t1)
    sw $t0, 0x500c($t1)
    # get the status of keypad
    lw $t0, 0x5004($t1)
    # display
    sw $t0, 0x5010($t1)
    sw $t0, 0x5014($t1)
    # loop
    b __start
    nop
disp_timer:
    # green light
    li $t0, 2
    lui $t1, 0xbfd0
    sw $t0, 0x5008($t1)
    sw $t0, 0x500c($t1)
    # get the value of timer
    lw $t0, 0x5018($t1)
    # display
    sw $t0, 0x5010($t1)
    sw $t0, 0x5014($t1)
    # loop
    b __start
    nop
