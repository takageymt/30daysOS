bits 32

global  api_putchar

SECTION .text

api_putchar:
    MOV EDX, 1
    MOV AL, [ESP+4]
    INT 0x40
    RET
