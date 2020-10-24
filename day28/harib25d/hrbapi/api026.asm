bits 32

global api_cmdline

section .text

api_cmdline:    ; int api_cmdline(char *buf, int maxsize);
    PUSH    EBX
    MOV     EDX, 26
    MOV     ECX, [ESP+12]
    MOV     EBX, [ESP+8]
    INT     0x40
    POP     EBX
    RET
