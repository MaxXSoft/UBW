.set noreorder
.global __start

__start:
setup_uart:
    # address of UART controller
    li $t0, 0xbfd01000
    # disable FIFO (FIFO capacity = 1 byte)
    sw $0, 0x008($t0)
    # set baudrate to 230400 (60MHz AXI clock)
    li $t1, 0x80
    sw $t1, 0x00c($t0)
    li $t1, 16   # 60,000,000 / (16 * 230400)
    sw $t1, 0x000($t0)
    sw $0, 0x004($t0)
    # set transmit format (8 bits/char)
    li $t1, 3
    sw $t1, 0x00c($t0)
    # disable interrupts & modem signals
    sw $0, 0x004($t0)
    sw $0, 0x010($t0)

get_data:
    li $t0, 0xbfd01000
    # check UART.LSR
    lw $t1, 0x014($t0)
    andi $t1, $t1, 0x01
    # data not ready
    beq $t1, $0, get_data
    nop
    # read & display data
    lw $v0, 0x000($t0)
    lui $t0, 0xbfd0
    b put_data
    sw $v0, 0x5014($t0)

put_data:
    li $t0, 0xbfd01000
    # check UART.LSR
    lw $t1, 0x014($t0)
    andi $t1, $t1, 0x20
    # data not ready
    beq $t1, $0, put_data
    nop
    # send data via UART
    b get_data
    sw $v0, 0x000($t0)
