#include"progressbar.h"

void progressbar()
{
    char bar[NUM];
    memset(bar, '\0', sizeof(bar) + 1);
    int cnt = 0;
    while(cnt <= NUM)
    {
        printf(RED"[%-100s]"END"[%d%%]\r", bar, cnt);
        fflush(stdout);
        bar[cnt++] = STYLE;
        usleep(1000);
    }
    printf("\n");
}

