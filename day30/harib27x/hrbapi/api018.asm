bits 32

global api_settimer

section .text

api_settimer:   ; void api_settimer(int timer, int time);
    PUSH    EBX
    MOV     EDX, 18
    MOV     EBX, [ESP+8]
    MOV     EAX, [ESP+12]
    INT     0x40
    POP     EBX
    RET
