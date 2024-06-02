#ifndef _PMBASE_H
#define _PMBASE_H

struct __double_pmgmem {
    unsigned char unused[384];
    unsigned char missiles[128];
    unsigned char player0[128];
    unsigned char player1[128];
    unsigned char player2[128];
    unsigned char player3[128];
};

static struct __double_pmgmem pmgmem;

#endif