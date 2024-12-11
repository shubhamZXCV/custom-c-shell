#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "init.h"
#include "global.h"
#include "prompt.h"
#include "handleInput.h"
#include "log.h"
#include "sig.h"
#include "activities.h"
#include "color.h"
#include"config.h"

int main()
{

  init();

  load_log();

  while (true)
  {

    setup_signal_handlers();
    prompt();
    if (fgets(INPUT, sizeof(INPUT), stdin) != NULL)
    {
    }
    else
    {
      save_log();
      for (int i = 0; i < process_count; i++)
      {
        if (strcmp(process_list[i].state, "Running") == 0 && process_list[i].fgbg == 'b')
        {
          if (kill(process_list[i].pid, SIGKILL) == -1)
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
            printf(GREEN "Sent signal %d to process with pid %d\n" GREEN, SIGKILL, process_list[i].pid);
          }
        }
      }
      exit(0);
    }

    if (strstr(INPUT, "exit"))
    {
      for (int i = 0; i < process_count; i++)
      {
        if (strcmp(process_list[i].state, "Running") == 0 && process_list[i].fgbg == 'b')
        {
          if (kill(process_list[i].pid, SIGKILL) == -1)
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
            printf(GREEN "Sent signal %d to process with pid %d\n" GREEN,SIGKILL, process_list[i].pid);
          }
        }
      }

      break;
    }
    strcpy(TWOSECMD, "");
    FOREGROUNDPID = -1;
 
    handleInput(INPUT);
  }

  save_log();

  return 0;
}