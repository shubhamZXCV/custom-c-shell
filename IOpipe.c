#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <error.h>
#include "color.h"
#include "command.h"
#include "utills.h"
#include "IO.h"

void iopipe(char *input, char process)
{
  int numOfPipes = count_char_occurrences(input, '|');
  int numOfCommands = numOfPipes + 1;

  pid_t pid;
  int pipefd[2 * numOfPipes];

  // Create all pipes
  for (int i = 0; i < numOfPipes; i++)
  {
    if (pipe(pipefd + i * 2) == -1)
    {
      perror("pipe");
      // Close any opened pipes before returning
      for (int j = 0; j < i; j++)
      {
        close(pipefd[2 * j]);
        close(pipefd[2 * j + 1]);
      }
      return;
    }
  }

  char *savePtr;
  char *cmd = strtok_r(input, "|", &savePtr);
  int i = 0;

  while (cmd != NULL && i < numOfCommands)
  {
    bool hasLessThan = (strstr(cmd, "<")) ? true : false;
    bool hasDoubleThan = (strstr(cmd, ">>")) ? true : false;
    bool hasGreaterThan = (strstr(cmd, ">")) ? true : false;
    hasGreaterThan = (hasDoubleThan) ? false : hasGreaterThan;
    pid = fork();

    if (pid < 0)
    {
      perror("fork");
      // Close all pipes before returning
      for (int j = 0; j < 2 * numOfPipes; j++)
      {
        close(pipefd[j]);
      }
      return;
    }

    if (pid == 0) // Child process
    {
      // If this is not the first command, redirect input from the previous pipe
      if (i > 0)
      {
        dup2(pipefd[(i - 1) * 2], STDIN_FILENO);
      }

      // If this is not the last command, redirect output to the current pipe
      if (i < numOfCommands - 1)
      {
        dup2(pipefd[2 * i + 1], STDOUT_FILENO);
      }

      // Close all pipe file descriptors in the child process
      for (int j = 0; j < 2 * numOfPipes; j++)
      {
        close(pipefd[j]);
      }

      // Execute the command
      trim_extra_spaces(cmd);
      if (i == numOfCommands - 1 && process == 'b')
      {
        IO(cmd, hasLessThan, hasGreaterThan, hasDoubleThan, 'b');
      }
      else
      {
        IO(cmd, hasLessThan, hasGreaterThan, hasDoubleThan, 'f');
      }
      exit(0); // Exit child process after executing the command
    }

    // Parent process: close pipes not needed for the next child
    if (i > 0) // Close the read end of the previous pipe
    {
      close(pipefd[(i - 1) * 2]);
    }
    if (i < numOfPipes) // Close the write end of the current pipe
    {
      close(pipefd[2 * i + 1]);
    }

    i++;
    cmd = strtok_r(NULL, "|", &savePtr);
  }

  // Close any remaining pipe file descriptors in the parent process
  for (int j = 0; j < 2 * numOfPipes; j++)
  {
    close(pipefd[j]);
  }

  // Wait for all child processes to finish
  int status;
  while (wait(&status) > 0)
  {
  }

  return;
}