bits 32

global api_openwin

section .text

api_openwin:    ; int api_openwin(char *buf, int xsz, int ysz, int col_inv, char *title);
    PUSH    EDI
    PUSH    ESI
    PUSH    EBX
    MOV     EDX, 5
    MOV     EBX, [ESP+16]   ; buf
    MOV     ESI, [ESP+20]   ; xsz
    MOV     EDI, [ESP+24]   ; ysz
    MOV     EAX, [ESP+28]   ; col_inv
    MOV     ECX, [ESP+32]   ; title
    INT     0x40
    ; AX = sht / return value
    POP     EBX
    POP     ESI
    POP     EDI
    RET
