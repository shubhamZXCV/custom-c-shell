#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include "global.h"
#include "hop.h"
#include "color.h"

void hop(char *hopCommand)
{
    char *buffer;
    buffer = (char *)malloc(sizeof(char) * BUF_SIZE);

    if (buffer == NULL)
    { // Check if malloc was successful
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    if (strlen(hopCommand) == 3)
    {
        getcwd(PREV_DIR, sizeof(PREV_DIR));
        if (chdir(HOME_DIR) == 0)
        {

            strcpy(CUR_DIR, HOME_DIR);
            printf("%s\n", CUR_DIR);
        }
        else
        {
            perror(RED "cant change dir\n" RESET);
        }
        return;
    }

    strcpy(buffer, hopCommand);

    char *savePtr2;
    char *token = strtok_r(buffer, " ", &savePtr2);

    while (token != NULL)
    {
        if (strcmp(token, "hop") == 0)
        {
            // Do nothing, just a command keyword
        }
        else if (strcmp(token, ".") == 0)
        {

            printf("%s\n", CUR_DIR);
        }
        else if (strcmp(token, "..") == 0)
        {
            getcwd(PREV_DIR, sizeof(PREV_DIR));
            if (chdir("..") == 0)
            {
                getcwd(CUR_DIR, sizeof(CUR_DIR));
                printf("%s\n", CUR_DIR);
            }
            else
            {
                perror(RED"can't change dir\n"RESET);
            }
        }
        else if (strcmp(token, "~") == 0)
        {
            getcwd(PREV_DIR, sizeof(PREV_DIR));
            if (chdir(HOME_DIR) == 0)
            {

                strcpy(CUR_DIR, HOME_DIR);
                printf("%s\n", CUR_DIR);
            }
            else
            {
                perror(RED"cant change dir\n"RESET);
            }
        }
        else if (strcmp(token, "-") == 0)
        {
            char *temp = (char *)malloc(sizeof(char) * 1024);
            getcwd(temp, 1024);
            if (chdir(PREV_DIR) == 0)
            {

                strcpy(CUR_DIR, PREV_DIR);
                printf("%s\n", PREV_DIR);
                strcpy(PREV_DIR, temp);
            }
            else
            {
                perror(RED"cant change dir\n"RESET);
            }
        }
        else
        {
            getcwd(PREV_DIR, sizeof(PREV_DIR));
            if (chdir(token) == 0)
            {
                getcwd(CUR_DIR, sizeof(CUR_DIR));
                printf("%s\n", CUR_DIR);
            }
            else
            {
                perror(RED"cant change dir\n"RESET);
            }
        }

        token = strtok_r(NULL, " ", &savePtr2);
    }
    free(buffer);
}
