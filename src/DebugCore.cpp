#include <common/core_aux.h>
#include <cstdio>
#include <common/core_globals.h>

#include "con_curses.h"
#include <common/core_helpers.h>

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
    // printf("New Real: %d" NL, new_real);
    printf("Stack Pointer: %p" NL, sp);
    printf("Big Stack: %d" NL, flags.f.big_stack);
    printf("Program Mode: %d" NL, flags.f.prgm_mode);
}

void dbg_print_phloat(const char *prefix, phloat value)
{
    if (!prefix)
        prefix = "";
    char buf[64];
    int len = easy_phloat2string(value, buf, sizeof(buf), 0);
    if (len < 0)
        len = 0;
    if (len >= (int)sizeof(buf))
        len = (int)sizeof(buf) - 1;
    buf[len] = '\0';
    printf("%s%s", prefix, buf);
}
