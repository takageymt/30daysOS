bits 32

global api_refreshwin

section .text

api_refreshwin:
    PUSH    EDI
    PUSH    ESI
    PUSH    EBX
    MOV     EDX, 12
    MOV     EBX, [ESP+16]
    MOV     EAX, [ESP+20]
    MOV     ECX, [ESP+24]
    MOV     ESI, [ESP+28]
    MOV     EDI, [ESP+32]
    INT     0x40
    POP     EBX
    POP     ESI
    POP     EDI
    RET
