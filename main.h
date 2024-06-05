#ifndef _MAIN_H
#define _MAIN_H

#define CHARSET (unsigned char *)0xE000
#define GROUND 60
#define DINOX 115
#define JUMPCLEAR 53
#define LIVES 3


//https://www.atariarchives.org/creativeatari/PlayerMissile_Graphics.php


unsigned char *src;
unsigned char *dst;

unsigned char scrlh;
unsigned char scrld;
unsigned char scrlm;
unsigned char scrlc=0;


unsigned char *dinoptr;
unsigned char y=0,oy=0;

unsigned char *curpm, *backpm, *temppm;

#pragma bss-name (push, "PM")
unsigned char pmgmembuf[0x800];
unsigned char pmgmembuf2[0x800];
unsigned char mapstrt[46];
unsigned char dlist[128];
static struct __double_pmgmem *pmgmem, *pmgmem2;
#pragma bss-name (pop)
#endif