.include "atari.inc"

; Entry point for C function
.global scrl
scrl:
    lda #7
    sta scrlh
    ldx #>scrli
    ldy #<scrli
    jmp SETVBV
scrli:
    pha
    txa
    pha
scrlit:
    ldx scrlh
    dex
    stx HSCROL
    stx scrlh
    bpl scrli3
    ldx #7
    stx scrlh
    stx HSCROL  
    ldx #0
    ldy scrlc
    iny
    sty scrlc
    cpy #22
    bcc loop2
    stx scrlc
    ldy #0
resloop:
    lda mapstrt,Y
    sta $0600+4,X
    lda mapstrt+1,Y
    sta $0600+5,X
    iny
    iny
    inx
    inx
    inx
    cpx #69
    bcc resloop
    jmp scrli3
    ldx #0
loop2:
    lda 0x0600+4,X
    clc
    adc #2
    sta $0600+4,X
    lda $0600+5,X
    adc #0
    sta $0600+5,X
    inx
    inx
    inx
    cpx #69
    bcc loop2
scrli3:
    pla
    tax
    pla
    jmp XITVBV  