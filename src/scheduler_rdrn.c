/* scheduler_rdrn.c: PQSH Round Robin Scheduler */

#include "pqsh/macros.h"
#include "pqsh/scheduler.h"
#include <errno.h>
#include <assert.h>

/**
 * Schedule next process using round robin policy:
 *
 *  1. If there no waiting processes, then do nothing.
 *
 *  2. Move one process from front of running queue and place in back of
 *  waiting queue.
 *
 *  3. Move one process from front of waiting queue and place in back of
 *  running queue.
 *
 * @param   s	    Scheduler structure
 **/
void scheduler_rdrn(Scheduler *s) {
    if (s->running.size == s->cores) {
        Process *p = queue_pop(&s->running);
        if (!process_pause(p)) {
            fprintf(stderr, "pause failed: %s\n", strerror(errno));
        }
        queue_push(&s->waiting, p);
    }

    while ((s->running.size < s->cores) && (s->waiting.size)) {
        Process *p = queue_pop(&s->waiting);
        if (p->pid == 0) {
            if (!process_start(p)) {
                fprintf(stderr, "start failed: %s\n", strerror(errno));
            }
        }
        else {
            if (!process_resume(p)) {
                fprintf(stderr, "resume failed: %s\n", strerror(errno));
            }
        }
        queue_push(&s->running, p);
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
