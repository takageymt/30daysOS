bits 32

global api_fseek

section .text

api_fseek:      ; void api_fseek(int fhandle, int offset, int mode);
    PUSH    EBX
    MOV     EDX, 23
    MOV     EAX, [ESP+8]
    MOV     ECX, [ESP+16]
    MOV     EBX, [ESP+12]
    INT     0x40
    POP     EBX
    RET
