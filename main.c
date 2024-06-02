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

    dlist[dl++] = DL_BLK8;
    dlist[dl++] = DL_BLK8;
    dlist[dl++] = DL_BLK8;
    for (int i = 0; i < 23; i++) {
        dlist[dl++] = DL_HSCROL(DL_LMS(DL_CHR40x8x1));
        mapstrt[i * 2] = dlist[dl++] = ((unsigned int)map + i * MAPCOLS) & 0xff;
        mapstrt[i * 2 + 1] = dlist[dl++] = mapstrt[i * 2 + 1] =
            ((unsigned int)map + i * MAPCOLS) >> 8 & 0xff;
    }
    dlist[dl++] = DL_JVB;
    dlist[dl++] = ((unsigned int)dlist) & 0xff;
    dlist[dl++] = ((unsigned int)dlist >> 8) & 0xff;
    OS.sdlst = dlist;
}

void initPat() {
    unsigned int i; 

    for (i = 0; i < PATLEN; i++) {
        POKE((unsigned char *)CHARSET + i, pat[i]);
    }
}

int game() {
    unsigned int score, x1, x2, x3;
    unsigned char key, lastjump, lives;
    lives = 3;
    lastjump = 0;
    score = 0;
    int yv = 0;

    initPat();

    OS.chbas = (unsigned char)((unsigned int)CHARSET / 256 & 0xff);

    OS.sdmctl = 46;
    OS.sdmctl = DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_MISSILES |
                DMACTL_DMA_PLAYERS | DMACTL_DMA_FETCH;

    OS.pcolr0 = COLOR_GREEN;
    OS.pcolr1 = COLOR_RED;
    OS.pcolr2 = COLOR_BLUE;
    OS.pcolr3 = COLOR_YELLOW;

    unsigned int i = OS.ramtop - 8;
    ANTIC.pmbase = i;
    pmgmem =
        (*(struct __double_pmgmem *)((unsigned int)(ANTIC.pmbase) * 256));

    OS.ramtop = i - 1;

    GTIA_WRITE.prior = PRIOR_P03_PF03;
    GTIA_WRITE.gractl = GRACTL_PLAYERS;

    GTIA_WRITE.sizep0 = PMG_SIZE_DOUBLE;
    GTIA_WRITE.sizep1 = PMG_SIZE_NORMAL;
    GTIA_WRITE.sizep2 = PMG_SIZE_NORMAL;
    GTIA_WRITE.sizep3 = PMG_SIZE_NORMAL;

    GTIA_WRITE.hposp0 = 115;
    GTIA_WRITE.hposp1 = x1 = 160;
    GTIA_WRITE.hposp2 = x2 = 150;
    GTIA_WRITE.hposp3 = x3 = 140;

    y = GROUND;

    updateDL();

    for (int j = 0; j < 128; j++) {
        pmgmem.missiles[j] = 0x00;
        pmgmem.player0[j] = 0x00;
        pmgmem.player1[j] = 0x00;
        pmgmem.player2[j] = 0x00;
        pmgmem.player3[j] = 0x00;
    }

    for (int j = 0; j < 8; j++) {
        pmgmem.player1[j + 46] = barrel1[j];
    }

    for (int j = 0; j < 8; j++) {
        pmgmem.player2[j + 46] = barrel1[j];
    }

    for (int j = 0; j < 8; j++) {
        pmgmem.player3[j + 46] = barrel1[j];
    }

    dinoptr = pmgmem.player0 + y;
    yv=-35;
    scrl();
    while (1) {
        oy = y;


        if (OS.ch == 0x21) {
            OS.ch = 0xff;
            y-=3;
        }


        if (y < 5) {
            y = 5;
        }

        if (yv < 0) {
            yv++;
        }

        if (y > GROUND) {
            y = GROUND;
            yv = 0;
        } else {
            yv++;
        }
        y+=yv/10;

        if (oy != y) {
            for (int j = oy; j < oy + 20; j++) {
                pmgmem.player0[j] = 0x00;
            }
            oy = y;

            for (int j = 0; j < 20; j++) {
                pmgmem.player0[j + y] = dino1[j];
            }
        }

        x1 -= 1;
        x2 -= 1;
        x3 -= 1;

        if (x1 < 0) {
            x1 = 160;
        }

        if (x2 < 0) {
            x2 = 160;
        }

        if (x3 < 0) {
            x3 = 160;
        }
        /*
        GTIA_WRITE.hposp1 = x1;
        GTIA_WRITE.hposp2 = x2;
        GTIA_WRITE.hposp3 = x3;
        */
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
    printf("Hello, world!\n");
    while (OS.ch == 0xff) {
    }
    printf("Starting game...\n");

    printf("Score: %d\n", game());
}
