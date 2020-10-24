bits 32

global api_fsize

section .text

api_fsize:      ; int api_fsize(int fhandle, int mode);
    MOV     EDX, 24
    MOV     EAX, [ESP+4]
    MOV     EBX, [ESP+8]
    INT     0x40
    RET
