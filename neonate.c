#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include "color.h"
#include <signal.h>

struct termios prev_terminos;

void neoexec(int time)
{

  setbuf(stdout, NULL);
  if (tcgetattr(STDIN_FILENO, &prev_terminos) == -1)
    perror("tcgetattr");

  struct termios raw = prev_terminos;
  raw.c_lflag &= ~(ICANON | ECHO);
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    perror("tcsetattr");
  int child = fork();

  if (child < 0)
  {
    perror("fork");
    return;
  }

  if (child == 0)
  {
    while (1)
    {

      FILE *f = fopen("/proc/sys/kernel/ns_last_pid", "r");
      char pid[10];
      fgets(pid, 10, f);
      printf("%s", pid);
      sleep(time);
    }
  }
  else
  {
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'x')
    {
      continue;
    }

    kill(child, SIGKILL);
  }
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &prev_terminos) == -1)
    perror("tcsetattr");
}

void neonate(char *cmd)
{
  char *token = strtok(cmd, " ");
  token = strtok(NULL, " ");

  if (token == NULL)
  {
    FILE *f = fopen("/proc/sys/kernel/ns_last_pid", "r");
    char pid[10];
    fgets(pid, 10, f);
    printf("%s", pid);
    return;
  }

  token = strtok(NULL, " ");

  if (token == NULL)
  {
    printf(RED "Insufficient Args!!!\n" RED);
    return;
  }

  int time = atoi(token);

  neoexec(time);
}
