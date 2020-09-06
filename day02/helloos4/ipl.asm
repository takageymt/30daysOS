; hello-os
; TAB=4

        ORG 0x7c00              ; memory address the program loaded

; For a common FAT12 formmated floppy disk.

        JMP     entry           ; jump to entry
        DB      0x90
        DB      "HELLOIPL"      ; name for the boot sector (8 bytes)
        DW      512             ; size of a sector (bytes)
        DB      1               ; size of a cluster (sectors)
        DW      1               ; starting sector of FAT
        DB      2               ; number of FAT
        DW      224             ; size of the root directory segment
        DW      2880            ; size of the disk
        DB      0xf0            ; media type
        DW      9               ; length of FAT segment
        DW      18              ; sectors per track
        DW      2               ; number of heads
        DD      0               ; for partitioning
        DD      2880            ; size of the disk again
        DB      0,0,0x29        ; OMAJINAI
        DD      0xffffffff      ; volume serial no.
        DB      "HELLO-OS   "   ; name for the disk (11 bytes)
        DB      "FAT12   "      ; name of a format
        RESB    18              ; filling next 18 bytes with 0

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

        DB      0x55, 0xaa      ; end of the boot sector
