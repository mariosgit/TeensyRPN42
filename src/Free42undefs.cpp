#include <Free42lib.h>

/* === Undefined functions ... just stubs === */

bool shell_clk24() {
    return false;
}


void shell_message(const char *message) {
    //
}


const char *shell_platform() {
    return NULL;
}

const char *shell_number_format() {
    return "."; //localeconv()->decimal_point;
}

int shell_date_format() {
    return 0;
}

double shell_random_seed() {
  return 0.78237947239847;
}


void shell_print(const char *text, int length,
  const char *bits, int bytesperline,
  int x, int y, int width, int height)
{
  printf("Shell print: %s\n", text);
}


void shell_beeper(int frequency) {
}

void shell_beeper(int frequency, int duration) {
}

int shell_wants_cpu() {
  return 0;
}

void shell_get_time_date(uint4 *tim, uint4 *date, int *weekday) {
  *tim = 12345600;
  *date = 20150825;
  *weekday = 2;
}


uint4 shell_get_mem() {
  return 16*1024; // Returns free mem
}

uint4 shell_milliseconds() {
  return 1;
}

void shell_request_timeout3(int delay) {
}

void shell_delay(int duration) {
  // usleep(duration * 1000);
}

void shell_powerdown() {
}

int4 shell_read_saved_state(void *buf, int4 bufsize) {
  return -1;
}





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

}

#endif