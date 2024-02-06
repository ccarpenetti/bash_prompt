/* queue.c: PQSH Queue */

#include "pqsh/macros.h"
#include "pqsh/queue.h"

#include <assert.h>

/**
 * Push process to back of queue.
 * @param q     Pointer to Queue structure.
 **/
void        queue_push(Queue *q, Process *p) {
    /* TODO: Implement */
    if (q->head == NULL){
        q->head = p;
        q->tail = p;
        q->size++;
    }
    else {
        q->tail->next = p;
        q->tail = p;
        q->size++;
    }
}

/**
 * Pop process from front of queue.
 * @param q     Pointer to Queue structure.
 * @return  Process from front of queue.
 **/
Process *   queue_pop(Queue *q) {
    /* TODO: Implement */

    if (q->head == NULL){
        return NULL;
    }
    else {
        Process *ptr = q->head;
        q->head = q->head->next;
        q->size--;
        ptr->next = NULL;
        return ptr;
    }
}

/**
 * Remove and return process with specified pid.
 * @param q     Pointer to Queue structure.
 * @param pid   Pid of process to return.
 * @return  Process from Queue with specified pid.
 **/
Process *   queue_remove(Queue *q, pid_t pid) {
    Process *ptr = q->head;
    Process *prev = NULL;

    while (ptr != NULL){
        if (ptr->pid == pid){
            if (prev == NULL){
                q->head = ptr->next;
                q->size--;
                return ptr;
            }
            prev->next = ptr->next;    
            q->size--;
            return ptr;
        }
        ptr = ptr->next;
        prev = prev->next;
    }

    return NULL;

}

/**
 * Dump the contents of the Queue to the specified stream.
 * @param q     Queue structure.
 * @param fs    Output file stream.
 **/
void        queue_dump(Queue *q, FILE *fs) {
    fprintf(fs, "%6s %-30s %-13s %-13s %-13s\n", 
                "PID", "COMMAND", "ARRIVAL", "START", "END");
    Process *ptr = q->head;

    while (ptr != NULL){
        fprintf(fs, "%6d %-30s %-13lf %-13lf %-13lf\n", ptr->pid, ptr->command, ptr->arrival_time, ptr->start_time, ptr->end_time);
        ptr = ptr->next;
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
