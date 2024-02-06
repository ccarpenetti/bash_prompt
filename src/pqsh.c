/* pqsh.c: Process Queue Shell */

#include "pqsh/macros.h"
#include "pqsh/options.h"
#include "pqsh/scheduler.h"
#include "pqsh/signal.h"

#include <errno.h>
#include <string.h>
#include <sys/time.h>

/* Global Variables */

Scheduler PQShellScheduler = {
    .policy    = FIFO_POLICY,
    .cores     = 1,
    .timeout   = 250000,
};

/* Help Message */

void help() {
    printf("Commands:\n");
    printf("  add    command    Add command to waiting queue.\n");
    printf("  status [queue]    Display status of specified queue (default is all).\n");
    printf("  help              Display help message.\n");
    printf("  exit|quit         Exit shell.\n");
}

/* Main Execution */

int main(int argc, char *argv[]) {
    Scheduler *s = &PQShellScheduler;

    if (!parse_command_line_options(argc, argv, s)) {
        return EXIT_FAILURE;
    }
    /* TODO: Register signal handlers */
    if (!signal_register(SIGALRM, 0, sigalrm_handler)) {
        return EXIT_FAILURE;
    }

    /* TODO: Start timer interrupt */
    struct itimerval interval = {
        .it_interval = { .tv_sec = 0, .tv_usec = s->timeout },
        .it_value    = { .tv_sec = 0, .tv_usec = s->timeout },
    };
    if (setitimer(ITIMER_REAL, &interval, NULL) < 0) {
        fprintf(stdout, "Timer interrupt failed: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    

    /* TODO: Process shell comands */
    while (!feof(stdin)) {
        char command[BUFSIZ]  = "";
        char argument[BUFSIZ] = "";

        printf("\nPQSH> ");

        while (!fgets(command, BUFSIZ, stdin) && !feof(stdin));

        chomp(command);
        
        /* TODO: Handle add and status commands */
        if (streq(command, "help")) {
            help();
        } else if (streq(command, "exit") || streq(command, "quit")) {
            break;
        } else if (sscanf(command, "add %[^\t\n]", argument) == 1) {
                scheduler_add(s, stdout, argument);
        } else if (streq(command, "status")) {
                scheduler_status(s, stdout, RUNNING||WAITING||FINISHED);
        } else if (sscanf(command, "status %[^\t\n]", argument) == 1) {
                if (streq(argument, "running")){
                    scheduler_status(s, stdout, RUNNING);
                } else if (streq(argument, "waiting")){
                    scheduler_status(s, stdout, WAITING);
                } else if (streq(argument, "finished")){
                    scheduler_status(s, stdout, FINISHED);
                } else {
                    printf("Unknown queue.\n");
                }
        }
        else if (strlen(command)) {
            printf("Unknown command: %s\n", command);
        }
    }

    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
