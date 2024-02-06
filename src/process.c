/* process.c: PQSH Process */

#include "pqsh/macros.h"
#include "pqsh/process.h"
#include "pqsh/timestamp.h"

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

/**
 * Create new process structure given command.
 * @param   command     String with command to execute.
 * @return  Pointer to new process structure
 **/
Process *process_create(const char *command) {
    
    //Create new Process struct
    Process *p = (Process *)calloc(1, sizeof(Process));
    
    if (p == NULL){
        return NULL;
    }

    
    strcpy(p->command, command);

    p->next = NULL;
    return p;
}

/**
 * Start process by forking and executing the command.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not starting the process was successful
 **/
bool process_start(Process *p) {
    
    //Fork the process
    pid_t pid = fork();
    int i = 0;

    if (pid == 0){ // child
        char *argv[MAX_ARGUMENTS] = {0};

        for (char *token = strtok(p->command, " "); token; token = strtok(NULL, " ")) {
            argv[i++] = token;
        }

        if (execvp(argv[0], argv) < 0){
            _exit(EXIT_FAILURE);
            return false;
        }
    }
    else if (pid == -1){ // error
        return false;
    }
    else { //parent
        p->pid = pid;
        p->start_time = timestamp();
    }
    
    return true;

}
    
/**
 * Pause process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_pause(Process *p) {
    int result = kill(p->pid, SIGSTOP);
    if (!result) {
        return 1; // return 1 if it suceeded
    }
    else {
        return 0; // return 0 if it failed
    }

}

/**
 * Resume process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_resume(Process *p) {
    int result = kill(p->pid, SIGCONT);
    if (!result) {
        return 1; // return 1 if succeeded
    }
    else {
        return 0; // return 0 if failed
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
