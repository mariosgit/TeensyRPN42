#pragma once

#ifndef USE_CURSES
#error "USE_CURSES must be defined for this file, needed for propper getchar reading, it's complicated. brew install ncurses "
#endif

#include "con_curses.h"

#include <cstdlib>
#include <thread>

/**
 * Some fake function for NATIVE environment
 */

void randomSeed(unsigned long seed);

int analogRead(int pin);

int millis();

int random(int min, int max);

void delay(int duration);
