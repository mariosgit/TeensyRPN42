#ifndef __CON_CURSES_H__
#define __CON_CURSES_H__

#ifdef USE_CURSES

#include <locale.h>
#include <curses.h>

// Handle macro collisions
static const int KEY_ENTER_CURS = KEY_ENTER;
static const int KEY_UP_CURS    = KEY_UP;
static const int KEY_DOWN_CURS  = KEY_DOWN;
static const int KEY_EXIT_CURS  = KEY_EXIT;

#undef KEY_ENTER
#undef KEY_UP
#undef KEY_DOWN
#undef KEY_EXIT

#define KEY_UP 18
#define KEY_DOWN 23
#define KEY_ENTER 13
#define KEY_TAB 9
#define KEY_ESC 0x1b

#define NL "\r\n"

#else // USE_CURSES

// #define NL "\r\n"
#define NL "\n"

#endif // USE_CURSES


#endif
