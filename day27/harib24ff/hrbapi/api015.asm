bits 32

global api_getkey

section .text

api_getkey:     ; int api_getkey(int mode);
    MOV     EDX, 15
    MOV     EAX, [ESP+4]
    INT     0x40
    RET
