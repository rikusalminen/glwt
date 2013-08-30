#include <time.h>

double glwtGetTime()
{
    struct timespec time;
    clock_gettime(
#ifdef LINUX
                  CLOCK_MONOTONIC_RAW,
#else
                  CLOCK_MONOTONIC,
#endif
                  &time);
    return (double)time.tv_sec + (double)time.tv_nsec * 1e-9;
}
