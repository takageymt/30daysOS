bits 32

global api_fclose

section .text

api_fclose:     ; void api_fclose(int fhandle);
    MOV     EDX, 22
    MOV     EAX, [ESP+4]
    INT     0x40
    RET
