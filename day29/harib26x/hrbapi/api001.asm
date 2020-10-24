bits 32

global api_putchar

section .text

api_putchar:    ; void api_putchar(int c);
    MOV EDX, 1
    MOV AL, [ESP+4]
    INT 0x40
    RET
