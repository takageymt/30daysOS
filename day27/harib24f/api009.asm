bits 32

global api_malloc

section .text

api_malloc:     ; int *api_malloc(int size);
    PUSH    EBX
    MOV     EDX, 9
    MOV     EBX, [CS:0x0020]
    MOV     ECX, [ESP+8]
    INT     0x40
    POP     EBX
    RET
