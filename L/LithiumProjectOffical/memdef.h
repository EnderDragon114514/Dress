#include <stdlib.h>
/*
#include "A:\rbvm.h"
#include "A:\memdef1.h"
#include "A:\memdef2.h"
#include "A:\memdef3.h"
*/
#ifndef AEOS_MEMDEF
#define AEOS_MEMDEF
#endif

#ifdef bgidm
void printc(char* _buf,...){
    /*char* outputBuf;
    va_list args;
    va_start(args, _buf);
    vsprintf(outputBuf, _buf, args);
    va_end(args);*/
    printf("Runtime error:No backend for console\n");
}
#define graphAvail 1
#else
#define printc printf
#define graphAvail 0
#endif
#define maxProg 7

#define PI 3.14159265358979323846

//*extern*/ int getkey();
/*extern*/ int memi[maxProg][128];
/*extern*/ float memf[maxProg][128];
/*extern*/ char memc[maxProg][128];
/*extern*/ int sectline[maxProg][64];//short interger to reduce memory usage

int getkey();
