.global _up
.global _down

.importzp       ptr1, tmp
.import         popax
.segment "ZEROPAGE"
foo: .res 1
.segment "CODE"
_up:
    sta ptr1
    stx ptr1+1
    ldy #$0
    lda (ptr1),y
    tax
    iny
uloop:
    lda (ptr1),y    
    dey
    sta (ptr1),y
    iny
    iny
    cpy #$80
    bne uloop
    txa
    dey
    sta (ptr1),y
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
    txa
    iny
    sta (ptr1),y
    ldx foo
    dex
    stx foo
    bne doloop
    rts;-------------------------------------------------------------------------------



