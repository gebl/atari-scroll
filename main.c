#include "main.h" 

#include <atari.h>
#include <peekpoke.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "asmsprite.h"
#include "game.h"
#include "pmbase.h"
#include "scrl.h"

void updateDL() {
    int i;
    int dl = 0;

    dlist[dl++] = DL_BLK8;
    dlist[dl++] = DL_BLK8;
    dlist[dl++] = DL_BLK8;

    dlist[dl++] = DL_LMS(DL_CHR40x8x1);
    mapstrt[0] = dlist[dl++] = ((unsigned int)map ) & 0xff;
    mapstrt[1] = dlist[dl++] = mapstrt[2 + 1] =
        ((unsigned int)map) >> 8 & 0xff;

    for (i = 1; i < 23; i++) {
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
        pm->player2[j + GROUND + 12] = barrel1[j];
        pm->player3[j + GROUND + 12] = barrel1[j];
    }
}

void showLives(char l) {
    char c;
    for (c=0; c<LIVES; c++) {
        if (l>0) {
            map[15+c] = 0x3;
            l--;
        } else {
            map[15+c] = 0x7;
        }
    }
}

int game() {
    int score;
    unsigned char lives;
    int yv = 0, m;
    char lastjump =0;

    lives = LIVES;
    score = 0;
    x1v = 1;
    x2v = 1;
    x3v = 1;

    OS.ch = 0xff;
    x1v = 1;
    x2v = 0;
    x3v = 0;

    scrl();
    while (lives > 0) {
        score++;
        x1v = x1v || (rand() % 2);
        x2v = x2v || (rand() % 5);
        x3v = x3v || (rand() % 10);
        oy = y;

        if (OS.ch == 0x21 && lastjump < 4) {
            OS.ch = 0xff;
            yv += -40;
            lastjump++;
        }
        yv += 1;

        y += yv / 10;

        if (y < 10) {
            y = 10;
        }

        if (y > GROUND) {
            y = GROUND;
            yv = 0;
            lastjump = 0;
        }

        m = y - oy;

        if (m < 0) {
            up(pmgmem->player0, abs(m));
            up(pmgmem2->player0, abs(m));
        } else if (m > 0) {
            down(pmgmem->player0, m);
            down(pmgmem2->player0, m);
        }
        if (x1v == 1 && x1 > DINOX - 5 && x1 < DINOX + 5 && y > JUMPCLEAR) {
            x1v = 0;
            x1 = DINOX - 10;
            lives--;
        }
        if (x2v == 1 && x2 > DINOX - 5 && x2 < DINOX + 5 && y > JUMPCLEAR) {
            x2v = 0;
            x2 = DINOX - 10;
            lives--;
        }
        if (x3v == 1 && x3 > DINOX - 5 && x3 < DINOX + 5 && y > JUMPCLEAR) {
            x3v = 0;
            x3 = DINOX - 10;
            lives--;
        }
        showLives(lives);
    }
    return score;
}

int main() {
    char j;
    int origdl;

    curpm = (unsigned char *)&pmgmembuf;
    backpm = (unsigned char *)&pmgmembuf2;

    ANTIC.pmbase = (unsigned char)((((unsigned int)curpm) >> 8) & 0xff);
    pmgmem = (struct __double_pmgmem *)curpm;
    pmgmem2 = (struct __double_pmgmem *)backpm;

    OS.chbas = (unsigned char)((unsigned int)CHARSET / 256 & 0xff);

    initPM(pmgmem);
    initPM(pmgmem2);
    initPat();
    printf("Initializing display list...\n");
    printf("OS.sdlst: %x\n", OS.sdlst);
    printf("DL: %x\n", dlist);
    while(OS.ch == 0xff) {
    }
    OS.ch = 0xff;
    origdl = OS.sdlst;

    OS.gprior = PRIOR_P03_PF03;

    GTIA_WRITE.sizep0 = PMG_SIZE_DOUBLE;
    GTIA_WRITE.sizep1 = PMG_SIZE_NORMAL;
    GTIA_WRITE.sizep2 = PMG_SIZE_NORMAL;
    GTIA_WRITE.sizep3 = PMG_SIZE_NORMAL;

    OS.sdmctl = 46;
    OS.sdmctl = DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_MISSILES |
                DMACTL_DMA_PLAYERS | DMACTL_DMA_FETCH;

    OS.pcolr0 = COLOR_GREEN;
    OS.pcolr1 = COLOR_RED;
    OS.pcolr2 = COLOR_BLUE;
    OS.pcolr3 = COLOR_YELLOW;

    GTIA_WRITE.hposp0 = DINOX;

    for (j = 0; j < 20; j++) {
        pmgmem->player0[j] = dino1[j];
    }

    for (j = 0; j < 20; j++) {
        pmgmem2->player0[j] = dino2[j];
    }

    down(pmgmem->player0, GROUND);
    down(pmgmem2->player0, GROUND);
    y = GROUND;
    printf("Starting game...\n");
    
    while (1) {
        updateDL();
        GTIA_WRITE.hposp1 = x1 = rand() % 110;
        GTIA_WRITE.hposp2 = x2 = rand() % 110;
        GTIA_WRITE.hposp3 = x3 = rand() % 110;
        GTIA_WRITE.gractl = GRACTL_PLAYERS;
        printf("Score: %d\n",game());
        GTIA_WRITE.gractl = 0;
        OS.sdlst = origdl;
        while (OS.ch == 0xff) {
        }
        OS.ch = 0xff;
    }
}
