bits 32

global api_closewin

section .text

api_closewin:   ; void api_closewin(int win);
    PUSH    EBX
    MOV     EDX, 14
    MOV     EBX, [ESP+8]
    INT     0x40
    POP     EBX
    RET
