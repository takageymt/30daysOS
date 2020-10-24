bits 32

    MOV     AL, 'A'
    CALL    2*8:0x00000d09
    RETF

fin:
    HLT
    JMP     fin
