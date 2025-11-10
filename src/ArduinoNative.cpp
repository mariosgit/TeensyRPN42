
#include "ArduinoNative.h"

#ifdef ENV_NATIVE

#include <random>

void randomSeed(unsigned long seed)
{
    // Implementation for seeding the random number generator
    std::srand(seed);
}

int analogRead(int pin)
{
    // Implementation for reading an analog value from a pin
    return 0;
}

int millis()
{
    // Implementation for getting the number of milliseconds since the program started
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
        .count();
}

int random(int min, int max)
{
    // Implementation for generating a random number between min and max
    return std::rand() % (max - min + 1) + min;
}

void delay(int duration)
{
    // Implementation for delaying the program for a specified duration
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
}


#endif // ENV_NATIVE


