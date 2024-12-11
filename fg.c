#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include "color.h"
#include"global.h"

void fg_command(pid_t pid) {
    if (kill(pid, 0) == -1) {
        perror("No such process found");
        return;
    }

    // Resume the process if it is stopped
    kill(pid, SIGCONT);

    // Bring the process to the foreground
    tcsetpgrp(STDIN_FILENO, getpgid(pid));

    // Wait for the process to complete or stop
    int status;
    while (waitpid(pid, &status, WUNTRACED) == -1 && errno == EINTR) {
        // Restart waitpid if interrupted by a signal
    }

    // Return control back to the shell
    

    if (WIFSTOPPED(status)) {
        printf("Process [%d] stopped\n", pid);
    } else if (WIFEXITED(status)) {
        printf("Process [%d] exited with status %d\n", pid, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Process [%d] killed by signal %d\n", pid, WTERMSIG(status));
    }
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    tcsetpgrp(STDIN_FILENO,getpgid(getpid()));
}

void fg(char *cmd)
{
  char *pidString = strtok(cmd, " ");
  pidString = strtok(NULL, " ");

  if (pidString == NULL)
  {
    printf(RED "Insufficient Args!!!\n" RED);
    return;
  }
  int pid = atoi(pidString);
  fg_command(pid);
}