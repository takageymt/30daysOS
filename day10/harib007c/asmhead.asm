; haribote-os
; TAB=4

BOTPAK  EQU     0x00280000  ; load address of bootpack
DSKCAC  EQU     0x00100000  ; disk cache
DSKCAC0 EQU     0x00008000  ; disk cache for real mode

; BOOT INFO
CYLS    EQU     0x0ff0      ; set by boot sector
LEDS    EQU     0x0ff1
VMODE   EQU     0x0ff2      ; info about x-bits color
SCRNX   EQU     0x0ff4      ; screen width
SCRNY   EQU     0x0ff6      ; screen height
VRAM    EQU     0x0ff8      ; address of graphic buffer
        
        ORG     0xc200

; video settings

        MOV     AL, 0x13
        MOV     AH, 0x00
        INT     0x10
        MOV     BYTE [VMODE], 8     ; referenced from C
        MOV     WORD [SCRNX], 320
        MOV     WORD [SCRNY], 200
        MOV     DWORD [VRAM], 0x000a0000

; Keyboard status
        
        MOV     AH, 0x02
        INT     0x16        ; call keyboard BIOS
        MOV     [LEDS], AL

; Disable PIC interrupt
;   to avoid hung-up (on AT compatible machines)
;   initialize PIC lator

        MOV     AL, 0xff
        OUT     0x21, AL
        NOP
        OUT     0xa1, AL

        CLI                 ;   disable interrupt in CPU-level

; setup A20GATE to enable CPU to access memory more than 1MB

        CALL    waitkbdout
        MOV     AL, 0xd1
        OUT     0x64, AL
        CALL    waitkbdout
        MOV     AL, 0xdf    ; enable A20
        OUT     0x60, AL
        CALL    waitkbdout

; protect mode

        LGDT    [GDTR0]
        MOV     EAX, CR0
        AND     EAX, 0x7fffffff ; bit31 to 0 (disable paging)
        OR      EAX, 0x00000001 ; bit0 to 1 (protect mode)
        MOV     CR0, EAX
        JMP     pipelineflush

pipelineflush:
        MOV     AX, 1*8         ; RW segment (32bits)
        MOV     DS, AX
        MOV     ES, AX
        MOV     FS, AX
        MOV     GS, AX
        MOV     SS, AX

; transport bootpack

        MOV     ESI, bootpack   ; source
        MOV     EDI, BOTPAK     ; destination
        MOV     ECX, 512*1024/4
        CALL    memcpy

; transport disk data
    
; boot sector

        MOV     ESI, 0x7c00     ; source
        MOV     EDI, DSKCAC     ; destination
        MOV     ECX, 512/4
        CALL    memcpy

; others
    
        MOV     ESI, DSKCAC0+512    ; source
        MOV     EDI, DSKCAC+512     ; destination
        MOV     ECX, 0
        MOV     CL, BYTE [CYLS]     ; load #cylinders
        IMUL    ECX, 512*18*2/4     ; #cylinders to bytes/4
        SUB     ECX, 512/4          ; sub for IPL
        CALL    memcpy

; start bootpack

        MOV     EBX, BOTPAK
        MOV     ECX, [EBX+16]
        ADD     ECX, 3
        SHR     ECX, 2              ; ECX >>= 2
        JZ      skip
        MOV     ESI, [EBX+20]       ; source
        ADD     ESI, EBX
        MOV     EDI, [EBX+12]       ; destinaion
        CALL    memcpy
skip:
        MOV     ESP, [EBX+12]       ; init stack
        JMP     DWORD 2*8:0x0000001b

waitkbdout:
        IN      AL, 0x64
        AND     AL, 0x02
        JNZ     waitkbdout
        RET

memcpy:
        MOV     EAX, [ESI]
        ADD     ESI, 4
        MOV     [EDI], EAX
        ADD     EDI, 4
        SUB     ECX, 1
        JNZ     memcpy
        RET

        
        ALIGNB  16
GDT0:
        RESB    8
        DW      0xffff,0x0000,0x9200,0x00cf ; RW segment (32bits)
        DW      0xffff,0x0000,0x9a28,0x0047 ; executable segment (32bits) for bootpack
        
        DW      0
GDTR0:
        DW      8*3-1
        DD      GDT0

        ALIGNB  16
bootpack:
