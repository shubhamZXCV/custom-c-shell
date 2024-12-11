#include <ctype.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"config.h"

void extract_redirections(const char *input, char *outputFile, char *inputFile, char *appendFile)
{
  // Initialize the character arrays with empty strings
  outputFile[0] = '\0';
  inputFile[0] = '\0';
  appendFile[0] = '\0';

  // Duplicate the input string to tokenize
  char *input_copy = strdup(input);
  char *token = strtok(input_copy, " ");

  while (token != NULL)
  {
    if (strcmp(token, ">") == 0)
    {
      // Get the next token as the output file
      token = strtok(NULL, " ");
      if (token != NULL)
      {
        strcpy(outputFile, token);
      }
    }
    else if (strcmp(token, ">>") == 0)
    {
      // Get the next token as the append file
      token = strtok(NULL, " ");
      if (token != NULL)
      {
        strcpy(appendFile, token);
      }
    }
    else if (strcmp(token, "<") == 0)
    {
      // Get the next token as the input file
      token = strtok(NULL, " ");
      if (token != NULL)
      {
        strcpy(inputFile, token);
      }
    }
    // Move to the next token
    token = strtok(NULL, " ");
  }

  // Free the duplicated string
  free(input_copy);
}



int count_char_occurrences(const char *str, char target)
{
  int count = 0;
  while (*str != '\0')
  {
    if (*str == target)
    {
      count++;
    }
    str++;
  }
  return count;
}

void trim_extra_spaces(char *str)
{
  int i = 0, j = 0;
  int len = strlen(str);


  while (i < len)
  {
    while (i < len && isspace((unsigned char)str[i]))
      i++;
    while (i < len && !isspace((unsigned char)str[i]))
      str[j++] = str[i++];
    while (i < len && isspace((unsigned char)str[i]))
      i++;
    if (i < len)
      str[j++] = ' ';
  }
  str[j] = '\0';
}
