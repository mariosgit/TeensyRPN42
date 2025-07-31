#include <Free42lib.h>
#include <Arduino.h>

/* === Undefined functions ... just stubs === */

bool shell_clk24()
{
    return false;
}

void shell_message(const char *message)
{
    //
}

const char *shell_platform()
{
    return NULL;
}

const char *shell_number_format()
{
    return "."; // localeconv()->decimal_point;
}

int shell_date_format()
{
    return 0;
}

double shell_random_seed()
{
    // Use a combination of analog reading and millis() to generate a seed
    randomSeed(analogRead(0) * millis());
    return ((double)random(0, 1000000) / 1000000.0);
}

void shell_print(const char *text, int length,
                 const char *bits, int bytesperline,
                 int x, int y, int width, int height)
{
    Serial.print("Shell print: ");
    Serial.println(text);
}

void shell_beeper(int frequency)
{
    // If you have a buzzer connected to a pin, you could use tone
    // For example, if buzzer is on pin 9:
    // tone(9, frequency);
}

void shell_beeper(int frequency, int duration)
{
    // If you have a buzzer connected to a pin, you could use tone
    // For example, if buzzer is on pin 9:
    // tone(9, frequency, duration);
}

int shell_wants_cpu()
{
    return 0;
}

void shell_get_time_date(uint4 *tim, uint4 *date, int *weekday)
{
    // For a real implementation, you would use a real-time clock (RTC) module
    // or sync time via serial. This is just a placeholder.
    uint32_t seconds = millis() / 1000;
    *tim = 12345600 + seconds; // Just add elapsed seconds to a base time
    *date = 20150825;          // Fixed date for now
    *weekday = 2;              // Fixed weekday for now
}

uint4 shell_get_mem()
{
// For STM32F4, you can get an approximation of free memory
// by checking the stack pointer against heap
#if defined(ARDUINO_ARCH_STM32)
    extern char _end;
    extern char _estack;
    char *heap_end = &_end;
    char *stack_ptr = (char *)__get_MSP();
    return stack_ptr - heap_end;
#else
    return 16 * 1024; // Returns default value if architecture is not recognized
#endif
}

uint4 shell_milliseconds()
{
    return millis(); // Use Arduino's millis function
}

void shell_request_timeout3(int delay)
{
    // Arduino doesn't have a direct equivalent, but we can use delay if needed
    // delay(delay);
}

void shell_delay(int duration)
{
    delay(duration); // Use Arduino's delay function
}

void shell_powerdown()
{
    // Arduino-specific power management would go here if needed
}

int4 shell_read_saved_state(void *buf, int4 bufsize)
{
    return -1; // No state saving for now, could use EEPROM or SD card in the future
}

// Teensy does not have file io ? Maybe use SDCard ?

#ifdef __cplusplus
extern "C"
{

    int _unlink(const char *pathname)
    {
        return -1;
    }
    int _open(const char *pathname, int flags)
    {
        return -1;
    }
    int _link(const char *oldpath, const char *newpath)
    {
        return -1;
    }
    // int _write(int fd, const char buf, size_t count)
    // {
    //     return -1;
    // }
}

#endif