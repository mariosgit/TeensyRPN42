
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Arduino.h>

// Include Free42 core files with full paths
#include "../lib/Free42lib/common/core_main.h"
#include "../lib/Free42lib/common/core_display.h"
#include "../lib/Free42lib/common/core_helpers.h"

static int ann_updown = 0;
static int ann_shift = 0;
static int ann_print = 0;
static int ann_run = 0;
static int ann_battery = 0;
static int ann_g = 0;
static int ann_rad = 0;

// Changed to bool to match the function signature
static bool enqueued;

int hp2ascii(char *dst, const char *src, int srclen)
{
    const char *esc;
    unsigned char c;
    int s, d = 0;
    for (s = 0; s < srclen; s++)
    {
        c = src[s];
        if (c >= 130 && c != 138)
            c &= 127;
        switch (c)
        {
        /* NOTE: this code performs the following 12 translations
         * that are not ASCII, but seem to be widely accepted --
         * that is, they looked OK when I tried them in several
         * fonts in Windows and Linux, and in Memo Pad on the Palm:
         *
         *   0: 247 (0367) divide
         *   1: 215 (0327) multiply
         *   8: 191 (0277) upside-down question mark
         *  17: 181 (0265) lowercase mu
         *  18: 163 (0243) sterling
         *  19: 176 (0260) degree
         *  20: 197 (0305) Aring
         *  21: 209 (0321) Ntilde
         *  22: 196 (0304) Aumlaut
         *  25: 198 (0306) AE
         *  28: 214 (0326) Oumlaut
         *  29: 220 (0334) Uumlaut
         *
         * Two additional candidates are these:
         *
         *  26: 133 (0205) ellipsis
         *  31: 149 (0225) bullet
         *
         * I'm not using those last two because support for them is not
         * as good: they exist in Times New Roman and Adobe Courier
         * (tested on Windows and Linux, respectively) and on the Palm,
         * but are missing from Windows Fixedsys (the default Notepad
         * font, so that is a concern!) and X11 lucidatypewriter and
         * fixed.
         * Note that 133 and 149 are both in the 128-159 range, that
         * is, the Ctrl+Meta range, which is unused in many fonts.
         * Eventually, I should probably support several translation
         * modes: raw, pure ASCII (only emit codes 32-126 and 10),
         * non-pure as below, and more aggressive non-pure (using the
         * ellipsis and fatdot codes, and maybe others). Then again,
         * maybe not. :-)
         */
        case 0:
            esc = "\367";
            break;
        case 1:
            esc = "\327";
            break;
        case 2:
            esc = "\\sqrt";
            break;
        case 3:
            esc = "\\int";
            break;
        case 4:
            esc = "\\gray1";
            break;
        case 5:
            esc = "\\Sigma";
            break;
        case 6:
            esc = ">";
            break;
        case 7:
            esc = "\\pi";
            break;
        case 8:
            esc = "\277";
            break;
        case 9:
            esc = "<=";
            break;
        case 11:
            esc = ">=";
            break;
        case 12:
            esc = "!=";
            break;
        case 13:
            esc = "\\r";
            break;
        case 14:
            esc = "v";
            break;
        case 15:
            esc = "->";
            break;
        case 16:
            esc = "<-";
            break;
        case 17:
            esc = "\265";
            break;
        case 18:
            esc = "\243";
            break;
        case 19:
            esc = "\260";
            break;
        case 20:
            esc = "\305";
            break;
        case 21:
            esc = "\321";
            break;
        case 22:
            esc = "\304";
            break;
        case 23:
            esc = "\\angle";
            break;
        case 24:
            esc = "E";
            break;
        case 25:
            esc = "\306";
            break;
        case 26:
            esc = "...";
            break;
        case 27:
            esc = "\\esc";
            break;
        case 28:
            esc = "\326";
            break;
        case 29:
            esc = "\334";
            break;
        case 30:
            esc = "\\gray2";
            break;
        case 31:
            esc = "\\bullet";
            break;
        case '\\':
            esc = "\\\\";
            break;
        case 127:
            esc = "|-";
            break;
        case 128:
            esc = ":";
            break;
        case 129:
            esc = "y";
            break;
        case 138:
            esc = "\\LF";
            break;
        default:
            dst[d++] = c;
            continue;
        }
        while (*esc != 0)
            dst[d++] = *esc++;
    }
    return d;
}

/* === Undefined functions ... just stubs === */

// Note: This function is already defined in Free42undefs.cpp
// double shell_random_seed()
// {
//     return 0.78237947239847;
// }

// Note: This function is already defined in Free42undefs.cpp
// void shell_print(const char *text, int length,
//                 const char *bits, int bytesperline,
//                 int x, int y, int width, int height)
// {
//     Serial.print("Shell print: ");
//     Serial.println(text);
// }

// Note: This function is already defined in Free42undefs.cpp
// void shell_beeper(int frequency, int duration)
// {
// }

// Note: These functions are already defined in Free42undefs.cpp
// int shell_wants_cpu()
// {
//     return 0;
// }
// 
// void shell_get_time_date(uint4 *tim, uint4 *date, int *weekday)
// {
//     *tim = 12345600;
//     *date = 20150825;
//     *weekday = 2;
// }
// 
// uint4 shell_get_mem()
// {
//     return 16 * 1024; // Returns free mem
// }

// Note: This function is already defined in Free42undefs.cpp
// uint4 shell_milliseconds()
// {
//     return millis();
// }

// Note: The following functions are already defined in Free42undefs.cpp
// void shell_request_timeout3(int delay)
// {
// }
// 
// void shell_delay(int duration)
// {
//     delay(duration);
// }
// 
// void shell_powerdown()
// {
// }
// 
// int4 shell_read_saved_state(void *buf, int4 bufsize)
// {
//     return -1;
// }

/* ======================= */
/* === LOCAL FUNCTIONS === */
/* ======================= */

#define LCDW 131
#define LCDH 16

#define NL "\n"


char disp[LCDH][LCDW + 1];

void disp_clear()
{
    int i;
    memset(disp, ' ', (LCDW + 1) * LCDH);
    for (i = 0; i < LCDH; i++)
        disp[i][LCDW] = 0;
}

int disp_annun()
{
    if (ann_updown)
    {
        Serial.print("v^");
    }
    else
        Serial.print("  ");

    Serial.print("  ");
    Serial.print(ann_shift ? "SHIFT" : "     ");
    Serial.print("  ");
    return 11; // number of chars displayed
}

void disp_print()
{
    Serial.println("-------------------------");
    disp_annun();
    Serial.println();
    int i;
    for (i = 0; i < LCDH; i++) {
        Serial.print(disp[i]);
        Serial.println();
    }
}

/* --------------------------- */
/* --- LOCAL FUNCTIONS END --- */
/* --------------------------- */

/* ===================== */
/* === Ifc functions === */
/* ===================== */

void skin_repaint_annunciator(int which, bool state)
{
}

void shell_blitter(const char *bits, int bytesperline, int x, int y, int width, int height)
{
    // Update display buffer
    for (int v = y; v < y + height; v++)
    {
        for (int h = x; h < x + width; h++)
        {
            int b = bits[v * bytesperline + (h >> 3)] & (1 << (h & 7));
            disp[v][h] = b ? '#' : ' ';
        }
    }

    disp_print();
}

void shell_annunciators(int updn, int shf, int prt, int run, int g, int rad)
{
    // Update annunciators without debug output
    if (updn != -1 && ann_updown != updn)
    {
        ann_updown = updn;
        skin_repaint_annunciator(1, ann_updown);
    }
    if (shf != -1 && ann_shift != shf)
    {
        ann_shift = shf;
        skin_repaint_annunciator(2, ann_shift);
    }
    if (prt != -1 && ann_print != prt)
    {
        ann_print = prt;
        skin_repaint_annunciator(3, ann_print);
    }
    if (run != -1 && ann_run != run)
    {
        ann_run = run;
        skin_repaint_annunciator(4, ann_run);
    }
    if (g != -1 && ann_g != g)
    {
        ann_g = g;
        skin_repaint_annunciator(6, ann_g);
    }
    if (rad != -1 && ann_rad != rad)
    {
        ann_rad = rad;
        skin_repaint_annunciator(7, ann_rad);
    }
    disp_print();
}

int shell_low_battery()
{
    int lowbat = 0;
    if (lowbat != ann_battery)
    {
        ann_battery = lowbat;
        skin_repaint_annunciator(5, ann_battery);
    }
    return lowbat;
}

int decimal_point = 1;

int shell_decimal_point()
{
    return decimal_point ? 1 : 0;
}

/* ------------------------- */
/* --- Ifc functions END --- */
/* ------------------------- */

void empty_keydown()
{
    bool dummy_enqueued;
    int dummy_repeat;
    bool keep_running;
    do
    {
        // Run the core keydown without debug output
        keep_running = core_keydown(0, &dummy_enqueued, &dummy_repeat);
    } while (keep_running);
}

#define LINELEN 32

// Removing redundant declaration of enqueued
// static int enqueued;

// Map a serial character to a calculator key code
int map_key(char c) {
    int calc_key = -1;
    switch(c) {
        // Map all characters to the appropriate calculator key codes
        case '0': calc_key = 34; break;
        case '1': calc_key = 29; break;
        case '2': calc_key = 30; break;
        case '3': calc_key = 31; break;
        case '4': calc_key = 24; break;
        case '5': calc_key = 25; break;
        case '6': calc_key = 26; break;
        case '7': calc_key = 19; break;
        case '8': calc_key = 20; break;
        case '9': calc_key = 21; break;
        
        // Operators
        case '+': calc_key = 37; break;
        case '-': calc_key = 32; break;
        case '*': calc_key = 27; break;
        case '/': calc_key = 22; break;
        case '.': calc_key = 35; break;
        
        // Control keys
        case '\r':
        case '\n': calc_key = 13; break; // Enter
        case 8:
        case 127:  calc_key = 17; break; // Backspace
        
        // Map other common keys to calculator functions
        case 'q': calc_key = 1; break;
        case 'w': calc_key = 2; break;
        case 'e': calc_key = 3; break;
        case 'r': calc_key = 4; break;
        case 't': calc_key = 5; break;
        case 'y': calc_key = 6; break;
        case 'a': calc_key = 7; break;
        case 's': calc_key = 8; break;
        case 'd': calc_key = 9; break;
        case 'f': calc_key = 10; break;
        case 'g': calc_key = 11; break;
        case 'h': calc_key = 12; break;
        case 'z':
        case 'x': calc_key = 13; break; // Enter alternatives
        case 'c': calc_key = 14; break;
        case 'v': calc_key = 15; break;
        case 'b': calc_key = 16; break;
        case 'n': calc_key = 17; break; // Backspace alternative
        case 'o': calc_key = 33; break;
        case 'p': calc_key = 36; break;
        
        default: break;
    }
    return calc_key;
}

// Process a key by sending it directly to the calculator core
void process_key(int key) {
    if (key > 0 && key <= 37) {
        int repeat;
        bool keep_running;
        
        // Process keydown and continue processing while needed
        do {
            keep_running = core_keydown(key, &enqueued, &repeat);
        } while (keep_running);
        
        // Process keyup and continue processing while needed
        do {
            keep_running = core_keyup();
        } while (keep_running);
    }
}

void main_loop()
{
    Serial.println("RPN42 init...");
    core_init(0, 0, NULL, 0); // Fixed to match the required parameters
    Serial.println("RPN42 Calculator Ready");
    Serial.println("Press keys to operate the calculator (0-9, +, -, *, /, etc.)");

    while (true)
    {
        // Process any pending core operations
        empty_keydown();
        
        // Check if serial data is available
        if (Serial.available() > 0)
        {
            char c = Serial.read();
            Serial.write(c); // Echo character for feedback
            
            // Map the character to calculator key code
            int key = map_key(c);
            
            // Process the key if valid
            if (key > 0) {
                process_key(key);
            }
        }
        
        // Small delay to prevent CPU hogging
        delay(10);
    }
}

// Arduino setup function - called once at startup
void setup()
{
    // Initialize Serial communication at 115200 baud rate
    Serial.begin(115200);
    while (!Serial) {
        ; // Wait for serial port to connect
    }
    
    Serial.println("Free42 STM32 RPN Calculator");
    Serial.println("Initializing...");
    
    // Clear display buffer
    disp_clear();
    
    // Setup pins or other hardware if needed
    
    // Start the main calculator loop
    main_loop();  // Note: This function doesn't return
}

// Arduino loop function - called repeatedly
void loop()
{
    // Main loop is handled in main_loop()
    // This won't be reached because main_loop() contains an infinite loop
}
