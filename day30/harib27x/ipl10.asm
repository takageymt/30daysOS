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
        MOV     BX, 18*2*CYLS-1 ; total sectors to read
        CALL    readfast        ; fast read

        MOV     BYTE [0x0ff0], CYLS  ; store a no. of readed cylinders
        JMP     0xc200          ; jump to OS
        
error:
        MOV     AX, 0
        MOV     ES, AX
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
fin:
        HLT                     ; halt a CPU
        JMP     fin             ; infinite-loop
msg:
        DB      0x0a, 0x0a      ; two new lines
        DB      "load error"
        DB      0x0a            ; new line
        DB      0

readfast:
; ES:read address, CH:cylinders, DH:head, CL:sector, BX:#sectors to read

        MOV     AX, ES          ; calc. max AL from ES
        SHL     AX, 3           ; real read address = ES<<4+0, so AX<<3 equals AH = <address>/512 (512bytes = sector size)
        AND     AH, 0x7f        ; AH&0x7f = AH%128 (128*512bytes = 64Kbytes)
        MOV     AL, 128         ; 
        SUB     AL, AH          ; AL = 128 - AH (#sectors between read address and the nearest 64KB border)

        MOV     AH, BL          ; calc. max AL from BX to AH
        CMP     BH, 0           ; if (BH != 0) AH = 18;
        JE      .skip1
        MOV     AH, 18

.skip1:
        CMP     AL, AH          ; if (AL > AH) AL = AH
        JBE     .skip2
        MOV     AL, AH
.skip2:

        MOV     AH, 19          ; calc. max AL from CL to AH
        SUB     AH, CL          ; AH = 19 - CL
        CMP     AL, AH          ; if (AL > AH) AL = AH;
        JBE     .skip3
        MOV     AL, AH
.skip3:
        
        PUSH    BX
        MOV     SI, 0           ; set retry counter to 0
retry:
        MOV     AH, 0x02        ; AH=0x02 : read from a disk
        MOV     BX, 0           ; buffer index 0
        MOV     DL, 0x00        ; dirve A:
        PUSH    ES
        PUSH    DX
        PUSH    CX
        PUSH    AX
        INT     0x13            ; call disk BIOS
        JNC     next            ; if no error
        ADD     SI, 1
        CMP     SI, 5           ; failed 5 times, error
        JAE     error
        MOV     AH, 0x00        ; reset a drive (flags, etc)
        MOV     DL, 0x00        ; drive a:
        INT     0x13            ; call disk BIOS
        POP     AX
        POP     CX
        POP     DX
        POP     ES
        JMP     retry

next:
        POP     AX
        POP     CX
        POP     DX
        POP     BX              ; ES
        SHR     BX, 5           ; per 16bytes to per 512bytes
        MOV     AH, 0
        ADD     BX, AX          ; BX += AL;
        SHL     BX, 5           ; per 512bytes to 16bytes
        MOV     ES, BX          ; ES += AL*0x20
        POP     BX
        SUB     BX, AX
        JZ      .ret
        ADD     CL, AL          ; advance to next sectors
        CMP     CL, 18          ; sector 18 = end of cylinder
        JBE     readfast
        MOV     CL, 1           ; reset sector 1
        ADD     DH, 1           ; change head to 1
        CMP     DH, 2           ; if done with head 1
        JB      readfast
        MOV     DH, 0           ; change head to 0
        ADD     CH, 1           ; advance to next cylinder
        JMP     readfast
.ret
        RET

        RESB    0x1fe-($-$$)    ; filling to 0x1fe with 0

        DB      0x55, 0xaa      ; end of the boot sector
