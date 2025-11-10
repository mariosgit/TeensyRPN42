#include <common/core_aux.h>
#include <cstdio>
#include <common/core_globals.h>

#include "con_curses.h"

void debug_core()
{
    fflush(stdout);

    char buf[128];
    reg2str(buf, sizeof(buf), AUX_REG_T);
    printf("T=%s " NL, buf);
    reg2str(buf, sizeof(buf), AUX_REG_Z);
    printf("Z=%s " NL, buf);
    reg2str(buf, sizeof(buf), AUX_REG_Y);
    printf("Y=%s " NL, buf);
    reg2str(buf, sizeof(buf), AUX_REG_X);
    printf("X=%s " NL, buf);
}
