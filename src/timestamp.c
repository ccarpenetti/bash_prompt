/* timestamp.c: PQSH Timestamp */

#include "pqsh/timestamp.h"

#include <time.h>
#include <sys/time.h>

/**
 * Return current timestamp as a double.
 *
 * Utilizes gettimeofday for precision (accounting for seconds and
 * microseconds) and falls back to time (only account for seconds) if that
 * fails.
 *
 * @return  Double representing the current time.
 **/
double timestamp() {
    /* TODO: Implement */
    struct timeval time_struct;
    double timestamp = 0;

    gettimeofday(&time_struct, NULL);

    if (!time_struct.tv_sec || !time_struct.tv_usec) {
        timestamp = time(NULL);

    }
    else {
        timestamp = time_struct.tv_sec + (time_struct.tv_usec/1000000.0);
    }

    return timestamp;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
