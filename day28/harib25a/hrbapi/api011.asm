bits 32

global api_point

section .text

api_point:      ; void api_point(int win, int x, int y, int col);
    PUSH    EDI
    PUSH    ESI
    PUSH    EBX
    MOV     EDX, 11
    MOV     EBX, [ESP+16]
    MOV     ESI, [ESP+20]
    MOV     EDI, [ESP+24]
    MOV     EAX, [ESP+28]
    INT     0x40
    POP     EBX
    POP     ESI
    POP     EDI
    RET
