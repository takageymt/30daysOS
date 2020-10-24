bits 32

global api_alloctimer

section .text

api_alloctimer: ; void api_alloctimer(void);
    MOV     EDX, 16
    INT     0x40
    RET
