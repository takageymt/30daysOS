bits 32

global  api_putchar
global  api_putstr0
global  api_end

SECTION .text

api_putchar:
    MOV EDX, 1
    MOV AL, [ESP+4]
    INT 0x40
    RET

api_putstr0:
    PUSH    EBX
    MOV     EDX, 2
    MOV     EBX, [ESP+8]
    INT     0x40
    POP     EBX
    RET
    
api_end:
    MOV EDX, 4
    INT 0x40

