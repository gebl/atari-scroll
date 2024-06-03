#ifndef _MAIN_H
#define _MAIN_H

extern unsigned int _CHARGEN_START__;
extern unsigned int _PM_START__;
extern unsigned int _MAIN_START__;

#define CHARSET (unsigned char *)_CHARGEN_START__
#define GROUND 60


//https://www.atariarchives.org/creativeatari/PlayerMissile_Graphics.php


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

unsigned char *curpm, *backpm, *temppm;

#pragma bss-name (push, "PM")
unsigned char pmgmembuf[0x1600];
#pragma bss-name (pop)

#endif