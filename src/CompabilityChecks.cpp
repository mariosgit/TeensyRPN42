#include <stdio.h>
#include <cstring>
#include <climits>

#include "CompabilityChecks.h"
#include "DebugCore.h"
#include <common/core_variables.h>
#include <common/core_main.h>
#include <common/core_globals.h>
#include <common/core_phloat.h>

namespace {

/**
 * Format a double value into fixed-point string representation
 */
void format_fixed(double value, int precision, char decimal_char, char *out, size_t len) {
    if (len == 0)
        return;
    if (precision < 0)
        precision = 0;
    if (precision > 9)
        precision = 9;

    size_t pos = 0;
    unsigned long long scale = 1ULL;
    for (int i = 0; i < precision; ++i) {
        if (scale > ULLONG_MAX / 10ULL) {
            precision = i;
            break;
        }
        scale *= 10ULL;
    }
    bool negative = value < 0.0;
    double abs_val = negative ? -value : value;
    double scaled_double = abs_val * (double) scale + 0.5;
    if (scaled_double < 0.0)
        scaled_double = 0.0;
    if (scaled_double > (double) ULLONG_MAX)
        scaled_double = (double) ULLONG_MAX;
    unsigned long long scaled = (unsigned long long) scaled_double;

    char fracbuf[10];
    for (int i = precision - 1; i >= 0; --i) {
        fracbuf[i] = (char) ('0' + (scaled % 10ULL));
        scaled /= 10ULL;
    }
    unsigned long long int_part = scaled;

    if (negative && pos < len - 1)
        out[pos++] = '-';

    char intbuf[32];
    int intlen = 0;
    do {
        intbuf[intlen++] = (char) ('0' + (int) (int_part % 10ULL));
        int_part /= 10ULL;
    } while (int_part > 0 && intlen < (int) sizeof(intbuf));
    for (int i = intlen - 1; i >= 0 && pos < len - 1; --i)
        out[pos++] = intbuf[i];

    if (precision > 0 && pos < len - 1) {
        out[pos++] = decimal_char;
        for (int i = 0; i < precision && pos < len - 1; ++i)
            out[pos++] = fracbuf[i];
    }

    out[pos] = '\0';
}

} // namespace

/**
 * Check if printf can handle float values
 */
bool checkStdio()
{
    bool result = true;

    const size_t szfSize = 32;
    // printf test for float support
    char szF[szfSize] = "";
    char decimal_char = flags.f.decimal_point ? '.' : ',';
    format_fixed(3.14, 2, decimal_char, szF, sizeof(szF));

    char expected[5] = {'3', decimal_char, '1', '4', '\0'};
    bool resultPrintf = (strcmp(szF, expected) == 0);
    printf("[DBG] Local formatter 3%c14 -> %s\n", decimal_char, szF);
    printf("[DBG] Local formatter check %s\n", resultPrintf ? "YES" : "NO");

    result &= resultPrintf;

    // scanf test for float support
    phloat p = 0;
    result &= (string2phloat(szF, szfSize, &p) == 0);
    bool resultScanf = (p > phloat(3.13) && p < phloat(3.15));
    printf("[DBG] Parsed value: ");
    p = p*1.014; // make num longer ....
    format_fixed(p, 9, decimal_char, szF, sizeof(szF));
    printf("%s\n", szF);
    printf("[DBG] Parser check %s\n", resultScanf ? "YES" : "NO");


    // test mini-scanf


    return result;
}

void other()
{
    printf("[DBG] init mcu sizeof(vartype_real)=%lu sizeof(phloat)=%lu allow_big_stack=%d auto_repeat=%d big_stack=%d stack=%p\n",
           (unsigned long)sizeof(vartype_real), (unsigned long)sizeof(phloat),
           core_settings.allow_big_stack ? 1 : 0,
           core_settings.auto_repeat ? 1 : 0,
           flags.f.big_stack ? 1 : 0,
           (void *)stack);

    // char szF[20] = "";
    // dtostrf(31434875.894537, 5, 2, szF);
    // printf("[DBG] Can printf float ??? (float)3.14 > %f. No ?\n", (double)3.1498347658943);  // NO!!
    // printf("[DBG] Try dtostrf %s\n", szF);

    // also test
    const char *sample = "634.874478";
    phloat parsed = 0;
    if (string2phloat(sample, (int) strlen(sample), &parsed) == 0) {
        dbg_print_phloat("[DBG] test string2phloat =", parsed);
        printf("\n");
    }
}