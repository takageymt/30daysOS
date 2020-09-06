; hello-os
; TAB=4

; For a common FAT12 formatted floppy disk

        DB      0xeb, 0x4e, 0x90
        DB      "HELLOIPL"      ; name for a boot sector (8 bytes). IPL (initial program loader) = bootstrap program
        DW      512             ; size of a sector. must be 512 (bytes).
        DB      1               ; size of a cluster. must be 1 (sector).
        DW      1               ; starting sector of FAT. normally, be the first sector.
        DB      2               ; number of FAT. must be 2.
        DW      224             ; size of a root directory segment. normally, be 224 (entries).
        DW      2880            ; size of a disk. must be 2880 (sectors = 1440KB / 512B).
        DB      0xf0            ; media type. must be 0xf0.
        DW      9               ; length of FAT segment. must be 9 (sectors)
        DW      18              ; sectors per track. must be 18.
        DW      2               ; number of heads.
        DD      0               ; for partitioning. if unused, be 0.
        DD      2880            ; size of a disk again.
        DB      0,0,0x29        ; OMAJINAI
        DD      0xffffffff      ; volume serial no.
        DB      "HELLO-OS   "   ; name for a disk (11 bytes).
        DB      "FAT12   "      ; name of a format.
        RESB    18              ; filling next 18 bytes with 0.

; Program body

        DB      0xb8, 0x00, 0x00, 0x8e, 0xd0, 0xbc, 0x00, 0x7c
        DB      0x8e, 0xd8, 0x8e, 0xc0, 0xbe, 0x74, 0x7c, 0x8a
        DB      0x04, 0x83, 0xc6, 0x01, 0x3c, 0x00, 0x74, 0x09
        DB      0xb4, 0x0e, 0xbb, 0x0f, 0x00, 0xcd, 0x10, 0xeb
        DB      0xee, 0xf4, 0xeb, 0xfd

; Output a message
        
        DB      0x0a, 0x0a      ; two new lines.
        DB      "hello, world"
        DB      0x0a            ; new line.
        DB      0               ; end of a message.

        RESB    0x1fe-($-$$)    ; filling to 0x1fe (512th sector of a disk) with 0.

        DB      0x55, 0xaa      ; end of a boot sector. must be 0x55, 0xaa.

; Not for a boot sector

        DB      0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
        RESB    4600
        DB      0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
        RESB    1469432

