#pragma once


#ifdef ENV_NATIVE

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

#endif
