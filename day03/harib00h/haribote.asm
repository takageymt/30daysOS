; haribote-os
; TAB=4

; BOOT INFO
CYLS    EQU     0x0ff0      ; set by boot sector
LEDS    EQU     0x0ff1
VMODE   EQU     0x0ff2      ; info about x-bits color
SCRNX   EQU     0x0ff4      ; screen width
SCRNY   EQU     0x0ff6      ; screen height
VRAM    EQU     0x0ff8      ; address of graphic buffer
        
        ORG     0xc200

        MOV     AL, 0x13
        MOV     AH, 0x00
        INT     0x10
        MOV     BYTE [VMODE], 8
        MOV     WORD [SCRNX], 320
        MOV     WORD [SCRNY], 200
        MOV     DWORD [VRAM], 0x000a0000

; Keyboard status
        
        MOV     AH, 0x02
        INT     0x16        ; call keyboard BIOS
        MOV     [LEDS], AL

fin:
        HLT
        JMP     fin
