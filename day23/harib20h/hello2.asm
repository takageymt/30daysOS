bits 32

SECTION .text

    MOV     EDX, 2
    MOV     EBX, msg
    INT     0x40
    MOV     EDX, 4
    INT     0x40

msg:
    DB      "hello", 0
