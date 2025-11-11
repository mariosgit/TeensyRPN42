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

    printf("Mode Number Entry: %d" NL, mode_number_entry);
    printf("Command Line Length: %d" NL, cmdline_length);
    printf("New Real: %d" NL, new_real);
    printf("Stack Pointer: %p" NL, sp);
    printf("Big Stack: %d" NL, flags.f.big_stack);
    printf("Program Mode: %d" NL, flags.f.prgm_mode);
}
