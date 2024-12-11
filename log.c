#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "global.h"
#include <unistd.h>

#include"handleInput.h"

Command command_log[MAX_COMMANDS];
int command_count = 0;

int new_command_idx = 0;
int old_command_idx = 0;

int is_valid_command(const char *cmd)
{
  return strlen(cmd) > 0;
}

int is_same_as_previous(const char *cmd)
{
  if (command_count == 0)
    return 0;
  int previous_index = (new_command_idx - 1 + MAX_COMMANDS) % MAX_COMMANDS; // Handle negative index
  return strcmp(command_log[previous_index].command, cmd) == 0;
}

void add_to_log(const char *cmd)
{
  if (is_same_as_previous(cmd) || !is_valid_command(cmd) || strcmp(cmd, "log") == 0 || strcmp(cmd, "log purge") == 0 || strstr(cmd, "log execute"))
  {
    return;
  }

  strncpy(command_log[new_command_idx].command, cmd, MAX_COMMAND_LENGTH);
  new_command_idx = (new_command_idx + 1) % MAX_COMMANDS;
  if (command_count < MAX_COMMANDS)
  {
    command_count++;
  }
  else
  {
    old_command_idx = (old_command_idx + 1) % MAX_COMMANDS; // Overwrite the oldest command
  }
}

void load_log()
{
  FILE *file = fopen(LOG_FILE, "r");
  if (!file)
    return;

  char buffer[MAX_COMMAND_LENGTH];
  while (fgets(buffer, MAX_COMMAND_LENGTH, file))
  {
    buffer[strcspn(buffer, "\n")] = 0;
    add_to_log(buffer);
  }

  fclose(file);
}

void save_log()
{
  if (chdir(HOME_DIR) != 0)
  {
    perror("unable to go to home dir\n");
  }
  FILE *file = fopen(LOG_FILE, "w");
  if (!file)
    return;

  if(command_count==0){
    fprintf(file, "");
    fclose(file);
    return;
  }

  int i = old_command_idx;
  do
  {
    fprintf(file, "%s\n", command_log[i].command);
    i = (i + 1) % MAX_COMMANDS;
  } while (i != new_command_idx);

  fclose(file);
}

void logShow()
{
  if (command_count == 0)
  {
    return;
  }
  printf("Command log:\n");
  int i = old_command_idx;
  int count = 1;
  do
  {
    printf("%d: %s\n", count++, command_log[i].command);
    i = (i + 1) % MAX_COMMANDS;
  } while (i != new_command_idx);
}

void logPurge()
{
  command_count = 0;
  new_command_idx = 0;
  old_command_idx = 0;
}

void logExecute(char *command)
{
  char *savePtr;
  char *token = strtok_r(command, " ", &savePtr);
  int num = -1;
  while (token != NULL)
  {
    if (strcmp("log", token) == 0)
    {
    }
    else if (strcmp(token, "execute") == 0)
    {
    }
    else
    {
      if (strlen(token) > 2)
      {
        perror("This shell only logs 15 recent commands");
        return;
      }
      else
      {
        if (strlen(token) == 1)
        {
          num = token[0] - '0';
        }
        else
        {
          if (token[0] == '1')
          {
            num = 10 + (token[1] - '0');
          }
        }
      }
    }
    token = strtok_r(NULL, " ", &savePtr);
  }

  if (num > 15)
  {
    return;
  }

  int idx = (new_command_idx - num) % MAX_COMMANDS;
  if (idx < 0)
  {
    idx = MAX_COMMANDS + idx;
  }
  handleInput(command_log[idx].command);
}
