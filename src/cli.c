#include "../include/minifim.h"

void cli(void)
{
    printf("Usage: minifim <command> [args]\n");
    printf("Commands:\n");
    printf(" init\t\t\t\tcreate the baseline\n");
    printf(" check\t\t\t\tcompare the disk against the baseline\n");
    printf(" accept <path>\t\tapprove the current state of one filei\n");
    printf(" update\t\t\t\tregenerate the whole baseline\n");
}
