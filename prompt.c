#include <stdio.h>
#include "global.h"
#include <string.h>
void prompt()
{
  if (strcmp(CUR_DIR,HOME_DIR)==0)
  {
    printf("\033[1;33m");
    printf("<");
    printf("%s", USERNAME);
    printf("@");
    printf("%s", HOSTNAME);
    printf(":");
    printf("~");
    printf(" %s",TWOSECMD);
    printf("> ");
    printf("\033[0m"); 
  }
  else if (strlen(CUR_DIR) > strlen(HOME_DIR))
  {
    printf("\033[1;33m");
    printf("<");
    printf("%s", USERNAME);
    printf("@");
    printf("%s", HOSTNAME);
    printf(":");
    printf("~");
    printf("%s",CUR_DIR+strlen(HOME_DIR));
    printf(" %s",TWOSECMD);
    printf("> ");
    printf("\033[0m"); 
  }
  else
  {
    printf("\033[1;33m");
    printf("<");
    printf("%s", USERNAME);
    printf("@");
    printf("%s", HOSTNAME);
    printf(":");
    printf("%s",CUR_DIR);
    printf(" %s",TWOSECMD);
    printf("> ");
    printf("\033[0m"); 
  }
}