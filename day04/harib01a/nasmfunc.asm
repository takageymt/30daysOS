; nasmfunc
; TAB=4

global  io_hlt
global write_mem8

SECTION .text

io_hlt:
    HLT
    RET

write_mem8:
    MOV     ECX, [ESP+4]
    MOV     AL, [ESP+8]
    MOV     [ECX], AL
    RET
