; ipl.asm
; TAB=4

CYLS    EQU     10              ; no. of read cylinders

        ORG     0x7c00          ; memory address the program loaded

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

; read from a disk
        
        MOV     AX, 0x0820      ; buffer address
        MOV     ES, AX
        MOV     CH, 0           ; cylinder 0
        MOV     DH, 0           ; head 0
        MOV     CL, 2           ; sector 2

readloop:
        MOV     SI, 0           ; set retry counter to 0
retry:
        MOV     AH, 0x02        ; AH=0x02 : read from a disk
        MOV     AL, 1           ; read 1 sector
        MOV     BX, 0           ; buffer index 0
        MOV     DL, 0x00        ; dirve A:
        INT     0x13            ; call disk BIOS
        JNC     next            ; if no error
        ADD     SI, 1
        CMP     SI, 5           ; failed 5 times, error
        JAE     error
        MOV     AH, 0x00        ; reset a drive (flags, etc)
        MOV     DL, 0x00        ; drive a:
        INT     0x13            ; call disk BIOS
        JMP     retry

next:
        MOV     AX, ES
        ADD     AX, 0x0020      ; 0x0020 = 512 / 16
        MOV     ES, AX          ; ADD ES, * is not exists.
        ADD     CL, 1           ; advance to next sector
        CMP     CL, 18          ; read sector 2 - 18 (cylinder 0)
        JBE     readloop
        MOV     CL, 1           ; reset sector 1
        ADD     DH, 1           ; change head to 1
        CMP     DH, 2           ; if done with head 1
        JB      readloop
        MOV     DH, 0           ; change head to 0
        ADD     CH, 1           ; advance to next cylinder
        CMP     CH, CYLS
        JB      readloop
        
fin:
        HLT                     ; halt a CPU
        JMP     fin             ; infinite-loop

error:
        MOV     SI,msg          ; set an index to the beginning of msg
putloop:
        MOV     AL, [SI]        ; load a character
        ADD     SI, 1           ; advance the index
        CMP     AL, 0           ; if the end of msg
        JE      fin
        MOV     AH, 0x0e        ; output one character
        MOV     BX, 15          ; color
        INT     0x10            ; call video BIOS
        JMP     putloop

msg:
        DB      0x0a, 0x0a      ; two new lines
        DB      "load error"
        DB      0x0a            ; new line
        DB      0

        RESB    0x1fe-($-$$)    ; filling to 0x1fe with 0

        DB      0x55, 0xaa      ; end of the boot sector
