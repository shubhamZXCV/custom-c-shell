#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "activities.h"
#include "color.h"
#include"proclore.h"

Process process_list[MAX_PROCESSES];
int process_count = 0;

void add_process(pid_t pid, const char *command, char bgfg)
{
    if (process_count < MAX_PROCESSES)
    {
        process_list[process_count].pid = pid;
        strcpy(process_list[process_count].command, command);
        strcpy(process_list[process_count].state, "Running");
        process_list[process_count].fgbg = bgfg;
        process_count++;
    }
    else
    {
        printf(RED "Process list is full!\n" RESET);
    }
}

void update_process_states()
{

    for (int i = 0; i < process_count; i++)
    {
     
        char status=get_process_status(process_list[i].pid,true);
        if (status=='R' || status=='S')
        {
            

            strcpy(process_list[i].state, "Running");
        }
        else if (status=='T')
        {
       
            strcpy(process_list[i].state, "Stopped");
        }
        else
        {
         
            strcpy(process_list[i].state, "Exited");
        }
      
    }
}

int compare_processes(const void *a, const void *b)
{
    Process *pa = (Process *)a;
    Process *pb = (Process *)b;
    return strcmp(pa->command, pb->command);
}

void activities()
{
    update_process_states(); // Update the state of each process

    // Sort the processes lexicographically by command name
    qsort(process_list, process_count, sizeof(Process), compare_processes);

    // Print the list of processes
    for (int i = 0; i < process_count; i++)
    {
        if (process_list[i].fgbg == 'b')
        {

            if (strcmp(process_list[i].state, "Exited") != 0)
            { // Skip exited processes
                printf("%d : %s - %s\n", process_list[i].pid, process_list[i].command, process_list[i].state);
            }
        }
    }
}