#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include "color.h"
#include <string.h>
#include <unistd.h>



void ping(char *command)
{
  
  char pidString[100], signalString[100];

  char *token = strtok(command, " ");
  int cnt = 0;
  while (token != NULL)
  {
    if (cnt == 1)
    {
      strcpy(pidString, token);
    }
    else if (cnt == 2)
    {
      strcpy(signalString, token);
    }

    cnt++;
    token = strtok(NULL, " ");
  }

  if (cnt < 3)
  {
    printf(RED "Insufficient Args!!!\n" RED);
    return;
  }

  int pid = atoi(pidString);
  int signal = atoi(signalString);
  signal %= 32;

  if (kill(pid, signal) == -1)
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
    printf(GREEN "Sent signal %d to process with pid %d\n" GREEN, signal, pid);
  }
}