#ifndef _MAIN_H
#define _MAIN_H


#define CHARSET (unsigned char *)0x8000
//#define DLIST   (unsigned char *)0x0600
//#define SDLIST  (unsigned char *)0x0230
//#define CHRPTR 756

//https://www.atariarchives.org/creativeatari/PlayerMissile_Graphics.php


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
unsigned char dlist[128];
unsigned char *dinoptr;
unsigned int y,oy;

#endif