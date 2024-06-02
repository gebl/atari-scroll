.include "atari.inc"
; Entry point for C function
.global _scrl

.import _dlist
.import _mapstrt
.import _scrlc
.import _scrlh

_scrl:
    lda #7; deferref vbank routine
    sta _scrlh; handy to initialize scrlh also :-)
    ldx #>scrli ; arguments for address of VBI
    ldy #<scrli
    jmp SETVBV ; OS register VBlank
scrli:
    pha
    txa
    pha
scrlit:
    ldx _scrlh
    dex
    stx HSCROL
    stx _scrlh
    bpl out
    ldx #7
    stx _scrlh
    stx HSCROL  
    ldx #0
    ldy _scrlc
    iny
    sty _scrlc
    cpy #50
    bcc loop2
    stx _scrlc
    ldy #0
resloop:
    lda _mapstrt,Y
    sta _dlist+4,X
    lda _mapstrt+1,Y
    sta _dlist+5,X
    iny
    iny
    inx
    inx
    inx
    cpx #69
    bcc resloop
    jmp out
    ldx #0
loop2:
    lda _dlist+4,X
    clc
    adc #2
    sta _dlist+4,X
    lda _dlist+5,X
    adc #0
    sta _dlist+5,X
    inx
    inx
    inx
    cpx #69
    bcc loop2
out:
    pla
    tax
    pla
    jmp XITVBV  