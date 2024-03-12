#include <Arduino.h>

#include <Free42lib.h>
#include <unistd.h>
#include <stdio.h>

#define LINELEN 256
static bool enqueued;
static int ann_updown = 0;
static int ann_shift = 0;
static int ann_print = 0;
static int ann_run = 0;
static int ann_battery = 0;
static int ann_g = 0;
static int ann_rad = 0;

#define NL "\n"

#define LCDW 131
#define LCDH 16

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
#ifdef UTF8_DISP
        P(11);
        P(12);
#else
        printf("v^");
#endif
    }
    else
        printf("  ");

    printf("  ");
    printf("%s", ann_shift ? "SHIFT" : "     ");
    printf("  ");
    return 11; // number of chars displayed
}

void disp_print()
{
    printf("-------------------------" NL);
    disp_annun();
    printf(NL);
    int i;
    for (i = 0; i < LCDH; i++)
        printf("%s" NL, disp[i]);
}

void empty_keydown()
{
    bool dummy1;
    int dummy2, keep_running;
    do
    {
        // printf("empty keydown:" NL);
        keep_running = core_keydown(0, &dummy1, &dummy2);
        // printf("end of empty keydown: keep_running=%i" NL, keep_running);
    } while (keep_running);
}

/* ===================== */
/* === Ifc functions === */
/* ===================== */

void skin_repaint_annunciator(int which, bool state)
{
}

void shell_blitter(const char *bits, int bytesperline, int x, int y, int width, int height)
{
    printf("Shell blitter" NL);

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
    printf("Shell annunciators" NL);
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

int curses_map_key(int key)
{
    int calc_key = -1;
    switch (key)
    {
    case 'q':
        calc_key = 1;
        break;
    case 'w':
        calc_key = 2;
        break;
    case 'e':
        calc_key = 3;
        break;
    case 'r':
        calc_key = 4;
        break;
    case 't':
        calc_key = 5;
        break;
    case 'y':
        calc_key = 6;
        break;
    case 'a':
        calc_key = 7;
        break;
    case 's':
        calc_key = 8;
        break;
    case 'd':
        calc_key = 9;
        break;
    case 'f':
        calc_key = 10;
        break;
    case 'g':
        calc_key = 11;
        break;
    case 'h':
        calc_key = 12;
        break;
    case KEY_ENTER:
    case 'z':
        calc_key = 13;
        break;
    case 'x':
        calc_key = 13;
        break;
    case 'c':
        calc_key = 14;
        break;
    case 'v':
        calc_key = 15;
        break;
    case 'E':
    case 'b':
        calc_key = 16;
        break;
    case KEY_BACKSPACE:
    case 'n':
        calc_key = 17;
        break;
    case KEY_UP:
        calc_key = 18;
        break;
    case KEY_DOWN:
        calc_key = 23;
        break;
    case KEY_TAB:
        calc_key = 28;
        break;
    case 'o':
        calc_key = 33;
        break;

    case '0':
        calc_key = 34;
        break;
    case '1':
        calc_key = 29;
        break;
    case '2':
        calc_key = 30;
        break;
    case '3':
        calc_key = 31;
        break;
    case '4':
        calc_key = 24;
        break;
    case '5':
        calc_key = 25;
        break;
    case '6':
        calc_key = 26;
        break;
    case '7':
        calc_key = 19;
        break;
    case '8':
        calc_key = 20;
        break;
    case '9':
        calc_key = 21;
        break;

    case '/':
        calc_key = 22;
        break;
    case '*':
        calc_key = 27;
        break;
    case '-':
        calc_key = 32;
        break;
    case '+':
        calc_key = 37;
        break;
    case '.':
        calc_key = 35;
        break;

    case 'p':
        calc_key = 36;
        break;
    default:
        break;
    }
    return calc_key;
}

// https://github.com/TeensyUser/doc/wiki/How-To---stdio-%28printf---scanf---stdout%29
Print *stdPrint = nullptr;
Stream *stdStream = nullptr;

extern "C"
{
    int _write(int file, const void *buf, size_t len)
    {
        Print *out;

        // Send both stdout and stderr to stdPrint
        if (file == STDOUT_FILENO || file == STDERR_FILENO)
        {
            out = stdPrint;
        }
        else
        {
            out = (Print *)file;
        }

        if (out == nullptr)
        {
            return len;
        }

        // Don't check for len == 0 for returning early, in case there's side effects
        return out->write((const uint8_t *)buf, len);
    }
    int _read(int file, void *buf, size_t len)
    {
        printf("read");
        Stream *in;

        if (file == STDIN_FILENO)
        {
            in = stdStream;
        }
        else
        {
            in = (Stream *)file;
        }

        if (in == nullptr)
        {
            return 0;
        }

        if (len == 0)
        {
            return 0;
        }

        // Non-blocking approach:
        int avail = in->available();
        if (avail <= 0)
        {
            return 0;
        }
        size_t toRead = avail;
        if (toRead > len)
        {
            toRead = len;
        }
        return in->readBytes((char *)buf, toRead);
    }
}
void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
    }

    stdPrint = &Serial;
    // stdStream = &Serial;

    printf("Hallo RPN !\n");

    core_init(0, 0, NULL, 0);
}

void loop()
{
    // Run until end
    empty_keydown();

    if (Serial.available())
    {
        int key = 0;
        int ch = Serial.read();
        if (ch)
        {
            // break; // exit
            // printf("ch: %c", ch);
            key = curses_map_key(ch);

            if (key > 0)
            {
                // Key pressed
                printf("key press %i" NL, key);
                int repeat, keep_running;
                keep_running = core_keydown(key, &enqueued, &repeat);
                printf("end of keydown: keep_running=%i  enqueued=%i  repeat=%i" NL, keep_running, enqueued, repeat);
                printf("keyup" NL);
                keep_running = core_keyup();
                printf("end of keyup: keep_running=%i" NL, keep_running);
            }
        }
       
    }
    // delay(100);
}