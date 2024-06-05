.include "atari.inc"

.global _up
.global _down
.global _swap
.global _x1
.global _x2
.global _x3
.global _x1v
.global _x2v
.global _x3v
.global _decBarrels

.importzp       ptr1, tmp
.import         popax
.import         _curpm
.import         _backpm

.segment "ZEROPAGE"
foo: .res 1
.segment "BSS"
_x1: .res 1
_x2: .res 1
_x3: .res 1
_x1v: .res 1
_x2v: .res 1
_x3v: .res 1

.segment "CODE"

_swap:
    ; Swap PMBase buffers
    ldx _curpm+1 ; the high byte of the pointer
    ldy _backpm+1 ; the high byte of the pointer
    stx _backpm+1
    sty _curpm+1
    sty PMBASE
    rts

_decBarrels:
    ldx _x1v
    cpx #0
    beq b2
    ldx _x1
    dex
    dex
    stx HPOSP1
    stx _x1
    cpx #30
    bne b2
    ldx #200
    stx _x1
    ldx #0
    stx _x1v
b2:
    ldx _x2v
    cpx #0
    beq b3
    ldx _x2
    dex
    dex
    stx HPOSP2
    stx _x2
    cpx #40
    bne b3
    ldx #200
    stx _x2
    ldx #0
    stx _x2v
b3:
    ldx _x3v
    cpx #0
    beq bexit
    ldx _x3
    dex
    dex
    stx HPOSP3
    stx _x3
    cpx #50
    bne bexit
    ldx #200
    stx _x3
    ldx #0
    stx _x3v
bexit:
    rts

_up:
    sta foo
    jsr popax
    sta ptr1
    stx ptr1+1
uoloop:
    ldy #$0
    lda (ptr1),y
    tax
    iny
uiloop:
    lda (ptr1),y    
    dey
    sta (ptr1),y
    iny
    iny
    cpy #$80
    bne uiloop
    txa
    dey
    sta (ptr1),y
    ldx foo
    dex
    stx foo
    bne uoloop
    rts

_down:
    sta foo
    jsr popax
    sta ptr1
    stx ptr1+1
doloop:
    ldy #$80
    lda (ptr1),y
    tax
    dey
diloop:
    lda (ptr1),y    
    iny
    sta (ptr1),y
    dey
    dey
    bne diloop
    
    lda (ptr1),y
    iny
    sta (ptr1),y
    dey
    txa
    sta (ptr1),y
    ldx foo
    dex
    stx foo
    bne doloop
    rts;-------------------------------------------------------------------------------



