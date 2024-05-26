#include "main.h"

#include <atari.h>
#include <peekpoke.h>
#include <stdint.h>
#include <stdio.h>

#include "game.h"
#include "pmbase.h"
#include "scrl.h"

void updateDL() {
    int dl = 0;
    POKE(DLIST + dl++, DL_BLK8);  // 8 blank scan lines
    POKE(DLIST + dl++, DL_BLK8);
    POKE(DLIST + dl++, DL_BLK8);
    for (int i = 0; i < 23; i++) {
        POKE(DLIST + dl++, DL_HSCROL(DL_LMS(DL_GRAPHICS0)));
        POKE(DLIST + dl++, ((unsigned int)map + i * MAPCOLS) & 0xff);
        POKE(DLIST + dl++, ((unsigned int)map + i * MAPCOLS) >> 8 & 0xff);
        mapstrt[i * 2] = ((unsigned int)map + i * MAPCOLS) & 0xff;
        mapstrt[i * 2 + 1] = ((unsigned int)map + i * MAPCOLS) >> 8 & 0xff;
    }

    POKE(DLIST + dl++, DL_JVB);

    POKE(DLIST + dl++, ((unsigned int)DLIST) & 0xff);
    POKE(DLIST + dl++, ((unsigned int)DLIST >> 8) & 0xff);

    POKE(SDLIST, ((unsigned int)DLIST) & 0xff);
    POKE(SDLIST + 1, ((unsigned int)DLIST >> 8) & 0xff);
}

void initPat() {
    unsigned int i;

    for (i = 0; i < PATLEN; i++) {
        POKE((unsigned char *)CHARSET + i, pat[i]);
    }
}

int game() {
    unsigned int frame;
    unsigned int score, score2;
    int yv = 0;
    unsigned char key, lastjump, lives;
    unsigned int y;
    score = 0;
    score2 = 0;
    lives = 3;
    lastjump = 0;
    key = 0;

    initPat();
    // POKE(CHRPTR, (unsigned char)((unsigned int)CHARSET / 256 & 0xff));
    OS.chbas = (unsigned char)((unsigned int)CHARSET / 256 & 0xff);
    updateDL();

    // POKE(SDMACTL, 46);  // Enable DMA for players and missiles
    OS.sdmctl = 46;
    // POKE(OS.pcolr0, COLOR_GREEN);    // Player Color.
    OS.pcolr0 = COLOR_GREEN;
    OS.pcolr1 = COLOR_RED;
    OS.pcolr2 = COLOR_BLUE;
    OS.pcolr3 = COLOR_YELLOW;
    // POKE(OS.pcolr1, COLOR_RED);    // Player Color.

    // unsigned int i = PEEK(RAMTOP) - 8;
    unsigned int i = OS.ramtop - 8;
    ANTIC.pmbase = i;
    // POKE(PMBASE, i);

    GTIA_WRITE.prior = PRIOR_P03_PF03;
    // POKE(GRACTL,GRACTL_PLAYERS);
    GTIA_WRITE.gractl = GRACTL_PLAYERS;

    // POKE(SIZE0, PMG_SIZE_DOUBLE );
    GTIA_WRITE.sizep0 = PMG_SIZE_DOUBLE;
    GTIA_WRITE.sizep1 = PMG_SIZE_NORMAL;
    GTIA_WRITE.sizep2 = PMG_SIZE_NORMAL;
    GTIA_WRITE.sizep3 = PMG_SIZE_NORMAL;
    

    // POKE(BITS0, 218);
    unsigned int mypmbase = i * 256;

    volatile struct __double_pmgmem pmgmem = (*(
        volatile struct __double_pmgmem *)((unsigned int)(ANTIC.pmbase) * 256));

    // POKE(HPOS0, 115);   // Horizontal Position
    GTIA_WRITE.hposp0 = 115;
    GTIA_WRITE.hposp1 = 160;
    GTIA_WRITE.hposp2 = 150;
    GTIA_WRITE.hposp3 = 140;
    

    y = 45;

    //    for (int j=0; j<20; j++) {
    //        POKE(mypmbase+512+j+y, dino1[j]);
    //    }
    //    for (int j=0; j<8; j++) {
    //        POKE(mypmbase+640+j+y+12, barrel1[j]);
    //    }

    frame = 0;

    scrl();

    for (int j = 0; j < 128; j++) {
        pmgmem.missiles[j] = 0x00;
        pmgmem.player0[j] = 0x00;
        pmgmem.player1[j] = 0x00;
        pmgmem.player2[j] = 0x00;
        pmgmem.player3[j] = 0x00;
    }

    for (int j = 0; j < 8; j++) {
        pmgmem.player1[j + 57] = barrel1[j];
    }

    for (int j = 0; j < 8; j++) {
        pmgmem.player2[j + 57] = barrel1[j];
    }

    for (int j = 0; j < 8; j++) {
        pmgmem.player3[j + 57] = barrel1[j];
    }
    while (1) {
        score++;

        for (int j = 0; j < 20; j++) {
            pmgmem.player0[j + y] = dino1[j];
        }
    }
    return score;
}

int main() {
    /*
         unsigned int i = PEEK(RAMTOP) - 9;
     printf("%x\n", i*256);
     unsigned int j = i * 256 + 1024;
     printf("%x\n", j);
     while (1) {}



         unsigned char *dl = (unsigned char *)(PEEK(560) + PEEK(561) * 256);
         for (int i = 0; i < 31; i++) {
             char v = PEEK(dl + i);
             printf("%d\n", v);
         }
     */
    game();
}
