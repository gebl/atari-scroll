#include "main.h"

#include <atari.h>
#include <peekpoke.h>
#include <stdint.h>
#include <stdio.h>

#include "game.h"
#include "pmbase.h"
#include "scrl.h"
#include "asmsprite.h"

void updateDL() {
    int i;
    int dl = 0;

    dlist[dl++] = DL_BLK8;
    dlist[dl++] = DL_BLK8;
    dlist[dl++] = DL_BLK8;
    for (i = 0; i < 23; i++) {
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

void initPM(struct __double_pmgmem *pm) {
    int j;
    for (j = 0; j < 128; j++) {
        pm->missiles[j] = 0x00;
        pm->player0[j] = 0x00;
        pm->player1[j] = 0x00;
        pm->player2[j] = 0x00;
        pm->player3[j] = 0x00;
    }

    for (j = 0; j < 8; j++) {
        pm->player1[j + GROUND + 12] = barrel1[j];
    }

    for (j = 0; j < 8; j++) {
        pm->player2[j + GROUND + 12] = barrel1[j];
    }

    for (j = 0; j < 8; j++) {
        pm->player3[j + GROUND + 12] = barrel1[j];
    }
}

int game() {
    int score, x1, x2, x3;
    unsigned char lastjump, lives;
    int yv = 0;
    unsigned int j;
    void *sprite,*sprite2;

    lives = 3;
    lastjump = 0;
    score = 0;

    OS.sdmctl = 46;
    OS.sdmctl = DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_MISSILES |
                DMACTL_DMA_PLAYERS | DMACTL_DMA_FETCH;

    OS.pcolr0 = COLOR_GREEN;
    OS.pcolr1 = COLOR_RED;
    OS.pcolr2 = COLOR_BLUE;
    OS.pcolr3 = COLOR_YELLOW;

    curpm = (unsigned char *)&pmgmembuf;
    backpm = (unsigned char *)&pmgmembuf2;

    printf("pmbase: %x\n", (curpm));
    printf("pmbase2: %x\n", (backpm));

    printf("Chargent start: %x\n", _CHARGEN_START__);
    printf("Main start: %x\n", _MAIN_START__);
    printf("CHARSET: %x\n", CHARSET);
    printf("----------\n");
    printf("high pmbase: %x\n", (((unsigned int)curpm) >> 8) & 0xff);
    printf("sizeof pmgmem: %x\n", sizeof(struct __double_pmgmem));
    printf("pmbase+sizeof pmgmem: %x\n",
           (unsigned char *)curpm + sizeof(struct __double_pmgmem));

    ANTIC.pmbase = (unsigned char)((((unsigned int)curpm) >> 8) & 0xff);
    pmgmem = (struct __double_pmgmem *)curpm;
    pmgmem2 = (struct __double_pmgmem *)backpm;

    printf("pmgmem: %x\n", pmgmem);
    printf("pmgmem2: %x\n", pmgmem2);
    printf("pmgmem->player0: %x\n", pmgmem->player0);
    printf("pmgmem2->player0: %x\n", pmgmem2->player0);

    while (OS.ch == 0xff) {
    }
    OS.ch = 0xff;

    oy = y = GROUND;
    OS.chbas = (unsigned char)((unsigned int)CHARSET / 256 & 0xff);

    initPM(pmgmem);
    initPM(pmgmem2);
    initPat();

    dinoptr = pmgmem->player0 + y;
    yv = -35;

    updateDL();
    scrl();

    // OS.gprior = PRIOR_P03_PF03|PRIOR_GFX_MODE_11;
    OS.gprior = PRIOR_P03_PF03;
    GTIA_WRITE.gractl = GRACTL_PLAYERS;

    GTIA_WRITE.sizep0 = PMG_SIZE_DOUBLE;
    GTIA_WRITE.sizep1 = PMG_SIZE_NORMAL;
    GTIA_WRITE.sizep2 = PMG_SIZE_NORMAL;
    GTIA_WRITE.sizep3 = PMG_SIZE_NORMAL;

    GTIA_WRITE.hposp0 = 115;
    GTIA_WRITE.hposp1 = x1 = 160;
    GTIA_WRITE.hposp2 = x2 = 150;
    GTIA_WRITE.hposp3 = x3 = 140;

    for (j = 0; j < 20; j++) {
        pmgmem->player0[j] = dino1[j];
    }

    for (j = oy; j < oy + 20; j++) {
        pmgmem2->player0[j] = 0x00;
    }
    for (j = 0; j < 20; j++) {
        pmgmem2->player0[j + y - 30] = dino1[j];
    }

    sprite=pmgmem->player0;
    sprite2=pmgmem->player1;

    while (1) {
        oy = y;
        down(sprite, 10);
        down(sprite2, 1);
        if (OS.ch == 0x21) {
            OS.ch = 0xff;
            y -= 3;
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
        y += yv / 10;


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
                temppm=curpm;
                curpm=backpm;
                backpm=temppm;
                ANTIC.pmbase = (unsigned char)((((unsigned int)curpm) >> 8) &
           0xff);
        */
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
    OS.ch = 0xff;
    printf("Starting game...\n");

    printf("Score: %d\n", game());
}
