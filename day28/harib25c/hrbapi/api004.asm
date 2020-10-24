bits 32

global api_end

section .text

api_end:        ; void api_end(void);
    MOV EDX, 4
    INT 0x40
