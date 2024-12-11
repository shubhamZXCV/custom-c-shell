#include "global.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "command.h"
#include <stdlib.h>
#include <unistd.h>
#include "log.h"
#include "utills.h"
#include "handleInput.h"
#include <sys/types.h>
#include <unistd.h>
#include "color.h"
#include "pipeline.h"
#include "IO.h"
#include "config.h"
#include "IOpipe.h"

void handleInput(char *input)
{
  if(strcmp(input,"\n")==0){
    return;
  }
  
  input[strcspn(input, "\n")] = 0;

  trim_extra_spaces(input);
  
 
  // printf("%s\n",input);

  bool flag = (strstr(input, "log")) ? true : false;
  if (!flag)
  {
    add_to_log(input);
  }

  // aliases
  input = replace_aliases_in_string(input);

  // foreground background
  char *savePtr1;
  char *command = strtok_r(input, ";", &savePtr1);

  while (command != NULL)
  {

    trim_extra_spaces(command);
    int ampCount = count_char_occurrences(command, '&');
    if (ampCount == 0)
    {

      bool hasLessThan = (strstr(command, "<")) ? true : false;
      bool hasDoubleThan = (strstr(command, ">>")) ? true : false;
      bool hasGreaterThan = (strstr(command, ">")) ? true : false;
      hasGreaterThan = (hasDoubleThan) ? false : hasGreaterThan;
      bool hasPipe = (strstr(command, "|")) ? true : false;
      // IO redirection

      if (!hasPipe && (hasGreaterThan || hasLessThan || hasDoubleThan))
      {

        IO(command, hasLessThan, hasGreaterThan, hasDoubleThan, 'f');
      }

      // pipelining
      else if (hasPipe && !hasGreaterThan && !hasLessThan && !hasDoubleThan)
      {
        pipeline(command, 'f');
      }

      else if (hasPipe && (hasGreaterThan || hasLessThan || hasDoubleThan))
      {
        iopipe(command, 'f');
      }
      else
      {
        whatCommand(command, 0);
      }
    }
    else
    {
      char *savePtr2;
      char *command2 = strtok_r(command, "&", &savePtr2);
      int count = 0;
      while (command2 != NULL)
      {
        bool hasLessThan = (strstr(command2, "<")) ? true : false;
        bool hasDoubleThan = (strstr(command2, ">>")) ? true : false;
        bool hasGreaterThan = (strstr(command2, ">")) ? true : false;
        hasGreaterThan = (hasDoubleThan) ? false : hasGreaterThan;
        bool hasPipe = (strstr(command2, "|")) ? true : false;
        count++;
        if (count <= ampCount)
        {
          trim_extra_spaces(command2);

          // IO redirection

          if (!hasPipe && (hasGreaterThan || hasLessThan || hasDoubleThan))
          {
            IO(command2, hasLessThan, hasGreaterThan, hasDoubleThan, 'b');
          }

          // pipelining
          else if (hasPipe && !hasGreaterThan && !hasLessThan && !hasDoubleThan)
          {
            pipeline(command2, 'b');
          }

          else if (hasPipe && (hasGreaterThan || hasLessThan || hasDoubleThan))
          {
            iopipe(command2, 'b');
          }
          else
          {

            whatCommand(command2, 1);
          }
        }
        else
        {
          trim_extra_spaces(command2);

          if (!hasPipe && (hasGreaterThan || hasLessThan || hasDoubleThan))
          {
            IO(command2, hasLessThan, hasGreaterThan, hasDoubleThan, 'f');
          }

          // pipelining
          else if (hasPipe && !hasGreaterThan && !hasLessThan && !hasDoubleThan)
          {
            pipeline(command2, 'f');
          }

          else if (hasPipe && (hasGreaterThan || hasLessThan || hasDoubleThan))
          {
            iopipe(command2, 'f');
          }
          else
          {
            whatCommand(command2, 0);
          }
        }
        command2 = strtok_r(NULL, "&", &savePtr2);
      }
    }

    command = strtok_r(NULL, ";", &savePtr1);
  }
}