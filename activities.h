#include<unistd.h>
#ifndef ACTIVITIES_H
#define ACTIVITIES_H

#define MAX_PROCESSES 500
typedef struct {
    pid_t pid;
    char command[256];
    char state[10];  // "Running" or "Stopped"
    char fgbg;
} Process;

extern Process process_list[MAX_PROCESSES];
extern int process_count;



void add_process(pid_t pid, const char *command,char fgbg);

void update_process_states();

int compare_processes(const void *a, const void *b) ;

void activities() ;





#endif