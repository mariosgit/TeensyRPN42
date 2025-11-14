#include <stdio.h>
#include <cstring>

#include "CompabilityChecks.h"
#include "DebugCore.h"
#include <common/core_variables.h>
#include <common/core_main.h>
#include <common/core_globals.h>

/**
 * Check if printf can handle float values
 */
bool checkStdio()
{
    bool result = true;

    // printf test for float support
    char szF[32] = "";

    snprintf(szF, sizeof(szF), "%.2f", 3.14);

    bool resultPrintf = (strcmp(szF, "3.14") == 0);
    printf("[DBG] Can printf float ??? (float)3.14 > %f. No ?\n", (double)3.1498347658943); // NO!!
    printf("[DBG] Can printf float ??? %s\n", resultPrintf ? "YES" : "NO");

    result &= resultPrintf;

    // scanf test for float support
    double p = 0.0;
    result &= (sscanf("3.14", "%lf", &p) == 1);
    bool resultScanf = (p > 3.13 && p < 3.15);
    printf("[DBG] Can scanf float ??? (float)3.14 > %f. No ?\n", (float)p);
    printf("[DBG] Can scanf float ??? %s\n", resultScanf ? "YES" : "NO");


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
    char buf[64];
    phloat p = 634.874478; // it's a double when USE_BCD is off
    sscanf(buf, "%le", &p);
    dbg_print_phloat("[DBG] test sscanf phloat =", p);
    printf("\n");
}