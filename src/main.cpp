#include <Arduino.h>

#include <Free42lib.h>

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
#define LCDH  16


char disp[LCDH][LCDW+1];


void disp_clear() {
  int i;
  memset(disp, ' ', (LCDW+1)*LCDH);
  for(i=0; i<LCDH; i++)
    disp[i][LCDW]=0;
}

int disp_annun() {
  if (ann_updown) {
#ifdef UTF8_DISP
    P(11); P(12);
#else
    printf("v^");
#endif  
  } else
    printf("  ");

  printf("  ");
  printf("%s", ann_shift?"SHIFT":"     ");
  printf("  ");
  return 11; // number of chars displayed
}

void disp_print() {
  printf("-------------------------" NL);
  disp_annun();
  printf(NL);
  int i;
  for(i=0; i<LCDH; i++)
    printf("%s" NL, disp[i]);
}

void empty_keydown() {
  bool dummy1;
  int dummy2, keep_running;
  do {
    // printf("empty keydown:" NL);
    keep_running = core_keydown(0, &dummy1, &dummy2);
    // printf("end of empty keydown: keep_running=%i" NL, keep_running);
  } while (keep_running);
}


/* ===================== */
/* === Ifc functions === */
/* ===================== */


void skin_repaint_annunciator(int which, bool state) {
}

void shell_blitter(const char *bits, int bytesperline, int x, int y, int width, int height) {
  printf("Shell blitter" NL);

  for (int v = y; v < y + height; v++) {
    for (int h = x; h < x + width; h++) {
      int b = bits[v * bytesperline + (h >> 3)] & (1 << (h & 7));
      disp[v][h] = b ? '#' : ' ';
    }
  }

  disp_print();
}

void shell_annunciators(int updn, int shf, int prt, int run, int g, int rad) {
  printf("Shell annunciators" NL);
    if (updn != -1 && ann_updown != updn) {
        ann_updown = updn;
        skin_repaint_annunciator(1, ann_updown);
    }
    if (shf != -1 && ann_shift != shf) {
        ann_shift = shf;
        skin_repaint_annunciator(2, ann_shift);
    }
    if (prt != -1 && ann_print != prt) {
        ann_print = prt;
        skin_repaint_annunciator(3, ann_print);
    }
    if (run != -1 && ann_run != run) {
        ann_run = run;
        skin_repaint_annunciator(4, ann_run);
    }
    if (g != -1 && ann_g != g) {
        ann_g = g;
        skin_repaint_annunciator(6, ann_g);
    }
    if (rad != -1 && ann_rad != rad) {
        ann_rad = rad;
        skin_repaint_annunciator(7, ann_rad);
    }
  disp_print();
}

int shell_low_battery() {
  int lowbat = 0;
  if (lowbat != ann_battery) {
    ann_battery = lowbat;
    skin_repaint_annunciator(5, ann_battery);
  }
  return lowbat;
}

int decimal_point = 1;

int shell_decimal_point() {
  return decimal_point ? 1 : 0;
}



void setup()
{
    core_init(0, 0, NULL, 0);
}


void loop()
{
    char s[LINELEN];

    // Run until end
    empty_keydown();

    printf("> ");
    if (!fgets(s, LINELEN, stdin))
    {
        // break;
    }

    int key = atoi(s);
    if (key > 0 && key <= 37)
    {
        // Press key
        printf("key press %i\n", key);
        int repeat, keep_running;
        keep_running = core_keydown(key, &enqueued, &repeat);
        printf("end of keydown: keep_running=%i  enqueued=%i  repeat=%i\n", keep_running, enqueued, repeat);
        printf("keyup\n");
        keep_running = core_keyup();
        printf("end of keyup: keep_running=%i\n", keep_running);
    }
}