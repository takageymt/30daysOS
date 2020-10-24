OUTPUT_FORMAT(binary)

SECTIONS {
    .head 0x0 : {
        LONG(128 * 1024)        /* stack + .data + heap | 4KB */
        LONG(0x69726148)        /* "Hari" */
        LONG(0)                 /* memarea | 4KB */
        LONG(0x0400)            /* initial ESP */
        LONG(SIZEOF(.data))     /* size of .data */
        LONG(LOADADDR(.data))   /* load address of .data */
        LONG(0xe9000000)        /* JMP */
        LONG(HariMain - 0x20)   /* relative address of entry address */
        /*LONG((SIZEOF(.data) + 0x0400 + 0x0f) & ~0x0f)         /* start address of heap */
        LONG(24 * 1024)         /* start address of heap */
    }

    .text : { *(.text) }

    /* VMA = 0x0400, LMA = end of .text */
    .data 0x400 : AT ( ADDR(.text) + SIZEOF(.text) ) {
        *(.data)
        *(.rodata*)
        *(.bss)
    } 

    /DISCARD/ : { 
        *(.eh_frame)
    }
}
