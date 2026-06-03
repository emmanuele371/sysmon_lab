#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>
#include <stdio.h>      // for printf() (Bonus B3)
#include <stdlib.h>     // for getloadavg()
#include <sys/sysctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>

/* Init: Wird nur einmal beim Start (iocInit) ausgefuehrt */
static long sysmonInit(aSubRecord *prec) {
    return 0;
}

/* Process: Wird bei jedem SCAN-Zyklus (hier alle 2 Sekunden) aufgerufen */
static long sysmonProcess(aSubRecord *prec) {
    
    /* 1. Load Average (1, 5 und 15 Minuten) */
    double loadavg[3] = {0.0, 0.0, 0.0};
    getloadavg(loadavg, 3);
    *(double *)prec->vala = loadavg[0]; /* LOAD1 */
    *(double *)prec->valb = loadavg[1]; /* LOAD5 */
    *(double *)prec->valc = loadavg[2]; /* LOAD15 */

    /* 2. CPU-Anzahl ermitteln */
    long ncpu = sysconf(_SC_NPROCESSORS_ONLN);
    /* Wichtig: Cast auf epicsInt32 (32-Bit), um Speicherkorruption auf macOS (64-Bit) zu vermeiden! */
    *(epicsInt32 *)prec->vald = (epicsInt32)ncpu; 

    /* 3. Physischer RAM via sysctl (in GiB umrechnen) */
    uint64_t memsize = 0;
    size_t len = sizeof(memsize);
    /* Bonus B3: Fehlerbehandlung (Rueckgabewert von sysctlbyname pruefen) */
    if (sysctlbyname("hw.memsize", &memsize, &len, NULL, 0) != 0) {
        printf("Fehler: hw.memsize konnte nicht gelesen werden\n");
    }
    *(double *)prec->vale = (double)memsize / (1024.0 * 1024.0 * 1024.0);

    /* 4. Uptime (Systemlaufzeit berechnen) */
    struct timeval boottime = {0, 0};
    size_t len_boot = sizeof(boottime);
    /* Bonus B3: Auch hier Rueckgabewert checken */
    if (sysctlbyname("kern.boottime", &boottime, &len_boot, NULL, 0) != 0) {
        printf("Fehler: kern.boottime konnte nicht gelesen werden\n");
    }
    
    struct timeval now;
    gettimeofday(&now, NULL);
    /* Laufzeit in Sekunden = Jetzige Zeit minus Boot-Zeit */
    *(double *)prec->valf = (double)(now.tv_sec - boottime.tv_sec);

    return 0;
}

/* Funktionen im EPICS-Registry anmelden */
epicsRegisterFunction(sysmonInit);
epicsRegisterFunction(sysmonProcess);
