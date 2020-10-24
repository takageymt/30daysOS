bits 32

global api_fread

section .text

api_fread:      ; int api_fread(char *buf, int maxsize, int fhandle);
    PUSH    EBX
    MOV     EDX, 25
    MOV     EAX, [ESP+16]
    MOV     ECX, [ESP+12]
    MOV     EBX, [ESP+8]
    INT     0x40
    POP     EBX
    RET
