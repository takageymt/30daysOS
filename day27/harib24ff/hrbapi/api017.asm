bits 32

global api_inittimer

section .text

api_inittimer:  ; void api_inittimer(int timer, int data);
    PUSH    EBX
    MOV     EDX, 17
    MOV     EBX, [ESP+8]
    MOV     EAX, [ESP+12]
    INT     0x40
    POP     EBX
    RET
