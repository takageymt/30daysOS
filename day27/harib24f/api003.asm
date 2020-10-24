bits 32

global api_putstr1

section .text

api_putstr1:    ; void api_putstr1(char *s, int l);
    PUSH    EBX
    MOV     EDX, 3
    MOV     EBX, [ESP+8]
    MOV     ECX, [ESP+12]
    INT     0x40
    POP     EBX
    RET
