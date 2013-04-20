#include <time.h>

uint64_t glwtGetTime()
{
    struct timespec_t time;
    clock_gettime(
#ifdef LINUX
                  CLOCK_MONOTONIC_RAW,
#else
                  CLOCK_MONOTONIC,
#endif
                  &time);
    return (uint64_t)1000000000*time.tv_sec + time.tv_nsec;
}

