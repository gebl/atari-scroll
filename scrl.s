.include "atari.inc"
; Entry point for C function
.global _scrl

.import _dlist
.import _mapstrt
.import _scrlc
.import _scrlh
.import _swap
.import _decBarrels

 bar:    .word   0

_scrl: ; initialize the scroll vbi, called from C
    lda #7; deferref vbank routine
    sta _scrlh; handy to initialize scrlh also :-)
    ldx #>scrli ; arguments for address of VBI
    ldy #<scrli
    jmp SETVBV ; OS register VBlank
scrli: ; VBI routine
    pha
    txa
    pha
scrlit:
    jsr _decBarrels
    ldx _scrlh
    dex
    stx HSCROL
    stx _scrlh
    bpl out ; if positive, let the antic do the scrolling, otherwise update the map location
    jsr _swap
    ; Reset the scroll counter
    ldx #7
    stx _scrlh
    stx HSCROL
    ldx #0
    ldy _scrlc
    iny
    sty _scrlc
    cpy #40
    bcc loop2
    stx _scrlc
    ldy #0
resloop:
    ; Reset the background
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
    ldx #30 ; skip scrolling the top of the screen.
loop2b: ; move screen pointers in DLIST
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
    bcc loop2b
out:
    pla
    tax
    pla
    jmp XITVBV  