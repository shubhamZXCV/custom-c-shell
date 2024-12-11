#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <error.h>
#include "color.h"
#include "command.h"
#include "utills.h"

void IO(char *input, bool hasLessThan, bool hasGreaterThan, bool hasDoubleThan, char process)
{
  char outputFile[256], inputFile[256], appendFile[256];

  extract_redirections(input, outputFile, inputFile, appendFile);

  int stdin_backup, stdout_backup;
  int inputFD, outputFD, appendFD;

  if (hasLessThan)
  {
    inputFD = open(inputFile, O_RDONLY);
    if (inputFD == -1)
    {
      perror(RED "Error opening input file" RED);
      return;
    }
    stdin_backup = dup(STDIN_FILENO);
    dup2(inputFD, STDIN_FILENO);
  }
  if (hasDoubleThan)
  {

    appendFD = open(appendFile, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (appendFD == -1)
    {
      perror(RED "Error opening append file" RED);
      return;
    }
    stdout_backup = dup(STDOUT_FILENO); // Backup the stdout here again
    dup2(appendFD, STDOUT_FILENO);
  }

  if (hasGreaterThan)
  {

    outputFD = open(outputFile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (outputFD == -1)
    {
      perror(RED "Error opening output file" RED);
      return;
    }
    stdout_backup = dup(STDOUT_FILENO);
    dup2(outputFD, STDOUT_FILENO);
  }

  int firstOccur[3] = {0};
  int len = strlen(input);
  for (int i = 0; i < len; i++)
  {
    if (input[i] == '<')
    {
      firstOccur[0] = 1;
      break;
    }

    if (input[i] == '>' && i + 1 < len && input[i + 1] == '>')
    {
      firstOccur[2] = 1;
      break;
    }
    if (input[i] == '>')
    {
      firstOccur[1] = 1;
      break;
    }
  }

  // Execution part
  if (firstOccur[0] == 1)
  {
    char *token = strtok(input, "<");
    trim_extra_spaces(token);
    if (process == 'f')
    {
      whatCommand(token, 0);
    }
    else
    {
      whatCommand(token, 1);
    }
  }
  else if (firstOccur[1] == 1)
  {
    char *token = strtok(input, ">");
    trim_extra_spaces(token);
    if (process == 'f')
    {
      whatCommand(token, 0);
    }
    else
    {
      whatCommand(token, 1);
    }
  }
  else
  {
    char *token = strtok(input, ">>");
    trim_extra_spaces(token);
    if (process == 'f')
    {
      whatCommand(token, 0);
    }
    else
    {
      whatCommand(token, 1);
    }
  }

  // Restoring file descriptors
  if (hasLessThan)
  {
    dup2(stdin_backup, STDIN_FILENO);
    close(inputFD);
    close(stdin_backup); // It's safe to close the backup after restoring
  }
  if (hasGreaterThan || hasDoubleThan)
  {
    dup2(stdout_backup, STDOUT_FILENO); // Restore stdout
    hasGreaterThan ? close(outputFD) : close(appendFD);

    close(stdout_backup); // Again, close backup after restoring
  }
  return;
}