; hello-os
; TAB=4

        ORG 0x7c00              ; memory address the program loaded

; For a common FAT12 formmated floppy disk.

        JMP     entry           ; jump to entry
        DB      0x90
        DB      "HELLOIPL"
        DW      512
        DB      1
        DW      1
        DB      2
        DW      224
        DW      2880
        DB      0xf0
        DW      9
        DW      18
        DW      2
        DD      0
        DD      2880
        DB      0,0,0x29
        DD      0xffffffff
        DB      "HELLO-OS   "
        DB      "FAT12   "
        RESB    18

; Program body

entry:
        MOV     AX, 0           ; init the accumulator
        MOV     SS, AX          ; init the stack segment register
        MOV     SP, 0x7c00      ; init the stack pointer
        MOV     DS, AX          ; init the data segment register
        MOV     ES, AX          ; init the extra segment register

        MOV     SI, msg         ; set the source index to msg (address)
putloop:
        MOV     AL, [SI]        ; copy a memory value to AL register (lower 8 bits of AX)
        ADD     SI, 1           ; advance an index for next reading
        CMP     AL, 0           ; 0 means the end of a message
        JE      fin
        MOV     AH, 0x0e        ; for interrupt
        MOV     BX, 15          ; color code for interrupt
        INT     0x10            ; call video BIOS
        JMP     putloop         ; loop

fin:
        HLT                     ; halt a CPU
        JMP     fin             ; infinite-loop

msg:
        DB      0x0a, 0x0a      ; two new lines
        DB      "hello, world"
        DB      0x0a            ; new line
        DB      0

        RESB    0x1fe-($-$$)    ; filling to 0x1fe with 0

        DB      0x55, 0xaa
        
; Out of boot sector

        DB      0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
        RESB    4600
        DB      0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
        RESB    1469432

