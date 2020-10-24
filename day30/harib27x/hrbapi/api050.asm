bits 32

global api_show_esp

section .text

api_show_esp:
    MOV     EDX, 50
    MOV     EAX, ESP
    INT     0x40
    RET
