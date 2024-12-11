#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include"color.h"
#include<string.h>
#include<stdlib.h>
void bg_command(pid_t pid) {
    if (kill(pid, 0) == -1) {
        perror("No such process found");
        return;
    }

    // Resume the process in the background
    kill(pid, SIGCONT);
    printf("Process [%d] is running in the background\n", pid);
}

void bg(char *cmd)
{
  char *pidString = strtok(cmd, " ");
  pidString = strtok(NULL, " ");

  if (pidString == NULL)
  {
    printf(RED "Insufficient Args!!!\n" RED, stderr);
    return;
  }
  int pid = atoi(pidString);
  bg_command(pid);
}