#ifndef _MAIN_H
#define _MAIN_H


#define CHARSET (unsigned char *)0x8000
#define DLIST   (unsigned char *)0x0600
#define SDLIST  (unsigned char *)0x0230
#define CHRPTR 756

//https://www.atariarchives.org/creativeatari/PlayerMissile_Graphics.php
//#define SDMACTL 559
//#define HPOS0   53248
//#define PCOL0 704
//#define PCOL1 705

//#define SIZE0 53256
//#define BITS0 53261
//#define RAMTOP 106
//#define PMBASE 54279
//#define GRACTL 53277

#define high_byte(a) *((char *)&a + 1)
#define low_byte(a) *((char *)&a)

static unsigned char *backbuffer;
unsigned char *src;
unsigned char *dst;

unsigned char scrlh;
unsigned char scrld;
unsigned char scrlm;
unsigned char scrlc=0;

unsigned char mapstrt[46];

#endif