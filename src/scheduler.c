/* scheduler.c: PQSH Scheduler */

#include "pqsh/macros.h"
#include "pqsh/scheduler.h"
#include "pqsh/timestamp.h"

#include <errno.h>
#include <sys/wait.h>

/**
 * Add new command to waiting queue.
 * @param   s	    Pointer to Scheduler structure.
 * @param   fs      File stream to write to.
 * @param   command Command string for new Process.
 **/
void scheduler_add(Scheduler *s, FILE *fs, const char *command) {
    Process *p = process_create(command);

    if (p == NULL){
        fprintf(fs, "Process creation failed: %s\n", strerror(errno));
    }

    queue_push(&s->waiting,p);
    p->arrival_time = timestamp();
    fprintf(fs, "Added process \"%s\" to waiting queue.\n", command);
}

/**
 * Display status of queues in Scheduler.
 * @param   s	    Pointer to Scheduler structure.
 * @param   fs      File stream to write to.
 * @param   queue   Bitmask specifying which queues to display.
 **/
void scheduler_status(Scheduler *s, FILE *fs, int queue) {
    fprintf(fs, "Running = %4lu, Waiting = %4lu, Finished = %4lu, Turnaround = %05.2lf, Response = %05.2lf\n", s->running.size,
            s->waiting.size,s->finished.size,s->total_turnaround_time/s->finished.size, s->total_response_time/s->finished.size);
    if ((RUNNING && queue) && (s->running.size)) {
        fprintf(fs, "\nRunning Queue:\n");
        queue_dump(&s->running, fs);
    }
    if ((WAITING && queue) && (s->waiting.size)) {
        fprintf(fs, "\nWaiting Queue:\n");
        queue_dump(&s->waiting, fs);
    }
    if ((FINISHED && queue) && (s->finished.size)) {
        fprintf(fs, "\nFinished Queue:\n");
        queue_dump(&s->finished, fs);
    }
}

/**
 * Schedule next process using appropriate policy.
 * @param   s	    Pointer to Scheduler structure.
 **/
void scheduler_next(Scheduler *s) {

    if (s->policy == 0) {
        scheduler_fifo(s);
    }
    else if (s->policy == 1) {
        scheduler_rdrn(s);
    }
}

/**
 * Wait for any children and remove from queues and update metrics.
 * @param   s	    Pointer to Scheduler structure.
 **/
void scheduler_wait(Scheduler *s) {
     /**
     *  - Remove process from queues.
     *  - Update Process metrics.
     *  - Update Scheduler metrics.
     **/
    pid_t pid;
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        Process *found = queue_remove(&s->running, pid);
        if (found == NULL){
            found = queue_remove(&s->waiting, pid);            
        }

        if (found == NULL){
            continue;
        }

        found->end_time = timestamp();
        s->total_turnaround_time += (found->end_time - found->arrival_time);
        s->total_response_time += (found->start_time - found->arrival_time);
        queue_push(&s->finished, found);
    }

}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
