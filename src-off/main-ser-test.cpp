#include <Arduino.h>
#include <stdio.h>

/**
 * Serial Terminal Test Program
 *
 * This simple program tests the serial/terminal connection between
 * the device and the computer.
 *
 * It prints periodic status messages and echoes back any received characters.
 * Each received character is displayed in three forms:
 *   - As the ASCII character
 *   - As a decimal value
 *   - As a hexadecimal value
 */

// Function prototype for freeMemory
extern "C" char *sbrk(int i);
int freeMemory();

// LED control settings
#define LED_ON_TIME 100
#define LED_OFF_TIME 900
#define PRINT_INTERVAL 1000

// Main program variables
unsigned long lastPrintTime = 0;
unsigned long lastBlinkTime = 0;
bool ledState = false;
uint32_t loopCounter = 0;

void setup()
{
    // Initialize serial at 115200 baud
    Serial.begin(115200);

    // Setup the onboard LED
    pinMode(LED_BUILTIN, OUTPUT);

    // Send an initial message
    delay(500); // Short delay to let Serial initialize
    Serial.println("\n\n------------------------------");
    Serial.println("TeensyRPN42 - Serial Test");
    Serial.println("------------------------------");
    Serial.println("Type any characters to see them echoed back");
    Serial.println("Program will print status messages every second");
}

void loop()
{
    unsigned long currentMillis = millis();

    // Increment our counter
    loopCounter++;

    // Blink the LED to show the program is running
    if (currentMillis - lastBlinkTime >= (ledState ? LED_ON_TIME : LED_OFF_TIME))
    {
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
        lastBlinkTime = currentMillis;
    }

    // Send periodic status message
    if (currentMillis - lastPrintTime >= PRINT_INTERVAL)
    {
        Serial.print("Status: Running for ");
        Serial.print(currentMillis / 1000);
        Serial.print(" seconds | Loop count: ");
        Serial.print(loopCounter);
        Serial.print(" | Free memory: ");
        Serial.print(freeMemory());
        Serial.println(" bytes");
        lastPrintTime = currentMillis;
    }

    // Process any incoming serial data
    if (Serial.available() > 0)
    {
        int incomingByte = Serial.read();

        // Echo the received byte in multiple formats
        Serial.print("Received: '");

        // Print as character if printable, otherwise show as <XX>
        if (incomingByte >= 32 && incomingByte <= 126)
        {
            Serial.print((char)incomingByte);
        }
        else
        {
            Serial.print("<");
            if (incomingByte < 16)
                Serial.print("0");
            Serial.print(incomingByte, HEX);
            Serial.print(">");
        }

        Serial.print("' | Dec: ");
        Serial.print(incomingByte);
        Serial.print(" | Hex: 0x");
        if (incomingByte < 16)
            Serial.print("0");
        Serial.println(incomingByte, HEX);
    }

    // Small delay to prevent CPU hogging
    delay(10);
}

// Function to estimate free memory - implementing a simple heap estimation
extern "C" char *sbrk(int i);
int freeMemory()
{
    char stack_dummy = 0;
    return &stack_dummy - sbrk(0);
}