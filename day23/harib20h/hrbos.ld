OUTPUT_FORMAT(binary)

/*INPUT(bootpack.o nasmfunc.o hankaku.o)*/
OUTPUT(bootpack.hrb)

/*ENTRY(HariMain)*/
/*STARTUP(bootpack.o)*/

SECTIONS {
    .head 0x0 : {
        LONG(64 * 1024)         /* stack + .data + heap | 4KB */
        LONG(0x69726148)        /* "Hari" */
        LONG(0)                 /* memarea | 4KB */
        LONG(0x310000)          /* initial ESP */
        LONG(SIZEOF(.data))     /* size of .data */
        LONG(LOADADDR(.data))   /* load address of .data */
        LONG(0xe9000000)        /* JMP */
        LONG(HariMain - 0x20)   /* relative address of entry address */
        LONG(0)                 /* size of heap */
    }

    .text : { *(.text) }

    /* VMA = 0x310000, LMA = end of .text */
    .data 0x310000 : AT ( ADDR(.text) + SIZEOF(.text) ) { 
        *(.data)
        *(.rodata*)
        *(.bss)
    }

    /DISCARD/ : { 
        *(.eh_frame)
    }
}
