; nasmfunc
; TAB=4
bits 32 ; not necessary

global  io_hlt, io_cli, io_sti, io_stihlt
global  io_in8, io_in16, io_in32
global  io_out8, io_out16, io_out32
global  io_load_eflags, io_store_eflags
global  load_gdtr, load_idtr
global  load_cr0, store_cr0
global  load_tr
global  asm_inthandler20
global  asm_inthandler21, asm_inthandler27, asm_inthandler2c
global  asm_inthandler0d
global  memtest_sub
global  farjmp, farcall
global  asm_hrb_api, start_app
extern  inthandler20
extern  inthandler21, inthandler27, inthandler2c
extern  inthandler0d
extern  hrb_api

SECTION .text

io_hlt:     ; void io_hlt(void);
    HLT
    RET

io_cli:     ; void io_cli(void);
    CLI
    RET

io_sti:     ; void io_stihlt(void);
    STI
    RET

io_stihlt:
    STI
    HLT
    RET

io_in8:     ; void io_in8(int port);
    MOV     EDX, [ESP+4]    ; port
    MOV     EAX, 0
    IN      AL, DX
    RET

io_in16:    ; void io_in16(int port);
    MOV     EDX, [ESP+4]    ; port
    MOV     EAX, 0
    IN      AX, DX
    RET

io_in32:    ; void io_in32(int port);
    MOV     EDX, [ESP+4]    ; port
    IN      EAX, DX
    RET

io_out8:    ; void io_out8(int port, int data);
    MOV     EDX, [ESP+4]    ; port
    MOV     AL, [ESP+8]     ; data
    OUT     DX, AL
    RET

io_out16:   ; void io_out16(int port, int data);
    MOV     EDX, [ESP+4]    ; port
    MOV     EAX, [ESP+8]     ; data
    OUT     DX, AX
    RET

io_out32:    ; void io_out32(int port, int data);
    MOV     EDX, [ESP+4]    ; port
    MOV     EAX, [ESP+8]     ; data
    OUT     DX, EAX
    RET

io_load_eflags:     ; int io_load_eflags(void);
    PUSHFD      ; PUSH EFLAGS
    POP     EAX
    RET

io_store_eflags:    ; void io_store_eflags(int eflags);
    MOV     EAX, [ESP+4]
    PUSH    EAX
    POPFD       ; POP EFLAGS
    RET

load_gdtr:     ; void load_gdtr(int limit, int addr);
    MOV     AX, [ESP+4]
    MOV     [ESP+6], AX
    LGDT    [ESP+6]
    RET

load_idtr:     ; void load_idtr(int limit, int addr);
    MOV     AX, [ESP+4]
    MOV     [ESP+6], AX
    LIDT    [ESP+6]
    RET

load_cr0:   ; int load_cr0(void);
    MOV     EAX, CR0
    RET

store_cr0:  ; void store_cr0(int cr0);
    MOV     EAX, [ESP+4]
    MOV     CR0, EAX
    RET

load_tr:    ; void load_tr(int tr);
    LTR     [ESP+4]
    RET

asm_inthandler20:
    PUSH    ES
    PUSH    DS
    PUSHAD
    MOV     EAX, ESP
    PUSH    EAX
    MOV     AX, SS
    MOV     DS, AX
    MOV     ES, AX
    CALL    inthandler20
    POP     EAX
    POPAD
    POP     DS
    POP     ES
    IRETD

asm_inthandler21:
    PUSH    ES
    PUSH    DS
    PUSHAD
    MOV     EAX, ESP
    PUSH    EAX
    MOV     AX, SS
    MOV     DS, AX
    MOV     ES, AX
    CALL    inthandler21
    POP     EAX
    POPAD
    POP     DS
    POP     ES
    IRETD

asm_inthandler27:
    PUSH    ES
    PUSH    DS
    PUSHAD
    MOV     EAX, ESP
    PUSH    EAX
    MOV     AX, SS
    MOV     DS, AX
    MOV     ES, AX
    CALL    inthandler27
    POP     EAX
    POPAD
    POP     DS
    POP     ES
    IRETD

asm_inthandler2c:
    PUSH    ES
    PUSH    DS
    PUSHAD
    MOV     EAX, ESP
    PUSH    EAX
    MOV     AX, SS
    MOV     DS, AX
    MOV     ES, AX
    CALL    inthandler2c
    POP     EAX
    POPAD
    POP     DS
    POP     ES
    IRETD

asm_inthandler0d:
    STI
    PUSH    ES
    PUSH    DS
    PUSHAD
    MOV     EAX, ESP
    PUSH    EAX
    MOV     AX, SS
    MOV     DS, AX
    MOV     ES, AX
    CALL    inthandler0d
    CMP     EAX, 0
    JNE     end_app
    POP     EAX
    POPAD
    POP     DS
    POP     ES
    ADD     ESP, 4
    IRETD

memtest_sub:    ; unsigned int memtest_sub(unsigned int start, unsigned int end);
    PUSH    EDI
    PUSH    ESI
    PUSH    EBX
    MOV     ESI, 0xaa55aa55         ; pat0 = 0xaa55aa55;
    MOV     EDI, 0x55aa55aa         ; pat1 = 0x55aa55aa;
    MOV     EAX, [ESP+12+4]         ; i = start;
mts_loop:
    MOV     EBX, EAX
    ADD     EBX, 0xffc              ; p = i + 0xffc;
    MOV     EDX, [EBX]              ; old = *p;
    MOV     [EBX], ESI              ; *p = pat0;
    XOR     DWORD [EBX], 0xffffffff ; *p ^= 0xffffffff;
    CMP     EDI, [EBX]              ; if (*p != pat1) goto fin;
    JNE     mts_fin
    XOR     DWORD [EBX], 0xffffffff ; *p ^= 0xffffffff;
    CMP     ESI, [EBX]              ; if (*p != pat0) goto fin;
    JNE     mts_fin
    MOV     [EBX], EDX              ; *p = old;
    ADD     EAX, 0x1000             ; i += 0x1000;
    CMP     EAX, [ESP+12+8]         ; if (i <= end) goto mts_loop;
    JBE     mts_loop
    POP     EBX
    POP     ESI
    POP     EDI
    RET
mts_fin:
    MOV     [EBX], EDX              ; *p = old;
    POP     EBX
    POP     ESI
    POP     EDI
    RET

farjmp:     ; void farjmp(int eip, int cs);
    JMP     FAR [ESP+4]
    RET

farcall:    ; void farcall(int eip, int cs);
    CALL    FAR [ESP+4]
    RET

asm_hrb_api:
    ; already CLI
    PUSH    DS
    PUSH    ES
    PUSHAD          ; store registers
    PUSHAD          ; arguments for hrb_api
    MOV     AX, SS
    MOV     DS, AX
    MOV     ES, AX
    CALL    hrb_api
    CMP     EAX, 0
    JNE     end_app
    ADD     ESP, 32
    POPAD           ; restore 
    POP     ES
    POP     DS
    IRETD
end_app:
    MOV     ESP, [EAX] ; restore OS ESP when app starting 
    POPAD   ; PUSHAD in the start of start_app
    RET

start_app:  ; void start_app(int eip, int cs, int esp, int ds, int *tss_esp0);
    PUSHAD  ; store all of 32bits registers
    MOV     EAX, [ESP+36]   ; EIP for app
    MOV     ECX, [ESP+40]   ; CS for app
    MOV     EDX, [ESP+44]   ; ESP for app
    MOV     EBX, [ESP+48]   ; DS/SS for app
    MOV     EBP, [ESP+52]   ; address of tss_esp0
    MOV     [EBP  ], ESP    ; store OS ESP when app starting
    MOV     [EBP+4], SS     ; store OS SS
    MOV     ES, BX
    MOV     DS, BX
    MOV     FS, BX
    MOV     GS, BX
    ; just like return from app's OS calling to app
    OR      ECX, 3  ; privilege level for app
    OR      EBX, 3  ; privilege level for app
    PUSH    EBX     ; SS for app
    PUSH    EDX     ; ESP for app
    PUSH    ECX     ; CS for app
    PUSH    EAX     ; EIP for app
    RETF
