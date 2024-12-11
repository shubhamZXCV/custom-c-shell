// signals.c
#include <stdio.h>
#include <unistd.h>
#include "sig.h"
#include "ping.h"
#include "global.h"
#include "color.h"
#include <errno.h>
#include<unistd.h>
#include<string.h>
#include"activities.h"
#include<signal.h>
#include<sys/types.h>
#include<stdlib.h>

// Signal handler for SIGINT (Ctrl+C)
void handle_sigint(int sig)
{
    // printf("%d\n",FOREGROUNDPID);
    // fflush(stdout);
    if (FOREGROUNDPID != -1)
    {

        if (kill(FOREGROUNDPID, SIGKILL) == -1)
        {
            if (errno == ESRCH)
            {
                printf(RED "No such process found\n" RESET);
            }
            else
            {
                perror(RED "Error sending signal" RESET);
            }
        }
        else
        {
            fprintf(stderr,"Process [%d] stopped\n",FOREGROUNDPID);
            fprintf(stderr,GREEN "Sent signal %d to process with pid %d\n" RESET, sig, FOREGROUNDPID);
        }
    }
}

void stop_process(pid_t pid) {
    for (int i = 0; i < process_count; i++) {
        if (process_list[i].pid == pid) {
            // strcpy(process_list[i].state, "Stopped");
            process_list[i].fgbg='b';
            printf("[%d]+ Stopped %s\n", i + 1, process_list[i].command);
        }
    }
}

void handle_sigstp(int sig)
{
    if(FOREGROUNDPID!=-1){
         if (kill(FOREGROUNDPID, SIGTSTP) == -1)
        {
            if (errno == ESRCH)
            {
                printf(RED "No such process found\n" RED);
            }
            else
            {
                perror(RED "Error sending signal" RED);
            }
        }
        else
        {
            stop_process(FOREGROUNDPID);
            
            fprintf(stderr,GREEN "Sent signal %d to process with pid %d\n" GREEN, sig, FOREGROUNDPID);
            fprintf(stderr,"Process [%d] moved to background\n",FOREGROUNDPID);
        }
    }
}

// Function to set up signal handlers
void setup_signal_handlers()
{
    struct sigaction sa_tstp;

   signal(SIGINT,handle_sigint);

   
    // Set up the sigaction structure for SIGTSTP
    sa_tstp.sa_handler = handle_sigstp; // Specify the handler function
    sigemptyset(&sa_tstp.sa_mask);      // Don't block any signals during the handler
    sa_tstp.sa_flags = 0;               // No special flags

   

    // Set the SIGTSTP (Ctrl+Z) signal handler
    if (sigaction(SIGTSTP, &sa_tstp, NULL) == -1) {
        perror("sigaction for SIGTSTP failed");
        exit(1);
    }
}