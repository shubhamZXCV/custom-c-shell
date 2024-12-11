#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "color.h"
#include <stdbool.h>
#include<sys/types.h>

char get_process_status(pid_t pid, bool outside)
{
  char path[40], status;
  FILE *fp;
  pid_t pgrp, tpgid;

  snprintf(path, sizeof(path), "/proc/%d/stat", pid);
  fp = fopen(path, "r");
  if (fp)
  {
    fscanf(fp, "%*d %*s %c %*d %*d %d %d", &status, &pgrp, &tpgid);
    fclose(fp);

    // Check foreground/background status
    char state_desc[20];
    if (status == 'R')
      strcpy(state_desc, "Running");
    else if (status == 'S')
      strcpy(state_desc, "Sleeping");
    else if (status == 'Z')
      strcpy(state_desc, "Zombie");
    else
      strcpy(state_desc, "Stopped");

    // Add "+" if it's a foreground process
    if (!outside)
    {

      if (getpgid(pid) == tcgetpgrp(STDIN_FILENO))
        printf("Process Status: %s+\n", state_desc); // Foreground
      else
        printf("Process Status: %s\n", state_desc); // Background
    }
    return status;
  }
  else
  {
    return 'X';
    perror(RED "Error opening stat file" RESET);

  }
}

void get_virtual_memory(pid_t pid)
{
  char path[40];
  FILE *fp;
  long vm_size;

  snprintf(path, sizeof(path), "/proc/%d/statm", pid);
  fp = fopen(path, "r");
  if (fp)
  {
    fscanf(fp, "%ld", &vm_size);
    fclose(fp);
    printf("Virtual Memory: %ld pages\n", vm_size);
  }
}

void get_executable_path(pid_t pid)
{
  char path[40], exe_path[1024];
  ssize_t len;

  snprintf(path, sizeof(path), "/proc/%d/exe", pid);
  len = readlink(path, exe_path, sizeof(exe_path) - 1);
  if (len != -1)
  {
    exe_path[len] = '\0';
    printf("Executable Path: %s\n", exe_path);
  }
  else
  {
    perror(RED "readlink" RESET);
  }
}

void get_process_info(pid_t pid)
{

  get_process_status(pid, false);
  printf("Process Group ID: %d\n", getpgid(pid));
  get_virtual_memory(pid);
  get_executable_path(pid);
}

void proclore(char *command)
{
  char *pidStr = NULL;
  if (strlen(command) > 9)
  {
    pidStr = command + 9;

    char *endptr;
    long pid;

    pid = strtol(pidStr, &endptr, 10);

    if (*endptr == '\0')
    {
      printf("pid: %ld\n", pid);
      get_process_info(pid);
    }
    else
    {
      printf("Conversion error, non-numeric characters found: %s\n", endptr);
      return;
    }
  }
  else
  {
    printf("pid: %d\n", getpid());
    get_process_info(getpid());
  }
}