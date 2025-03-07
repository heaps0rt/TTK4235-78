#include "timer.h"
#include <sys/time.h>
#include <stddef.h>  // Added for NULL definition

double timer_get_current_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

int timer_expired(double deadline) {
    return timer_get_current_time() >= deadline;
}