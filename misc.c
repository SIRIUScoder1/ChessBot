// misc.c

//used for getting and storing the time

#include "stdio.h"
#include "defs.h"
#include "string.h"

#ifdef WIN32
#include "windows.h"
#else
#include "sys/time.h"
#endif

int GetTimeMs() {
#ifdef WIN32
        return GetTickCount();   // time elapsed since system has started
#else
        struct timeval t;
        gettimeofday(&t, NULL);
        return t.tv_sec*1000 + t.tv_usec/1000;
#endif
}
