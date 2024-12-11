
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "proclore.h"
#include "seek.h"
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include "color.h"
#include "iman.h"
#include "ping.h"
#include "fg.h"
#include "activities.h"
#include "neonate.h"
#include "bg.h"
#include"config.h"

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_COUNT 100

// Signal handler to handle SIGCHLD
void handle_sigchld(int sig)
{
    int status;
    pid_t pid;

    // Reap all child processes that have terminated
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf(GREEN "background process %d exited with status %d\n" RESET, pid, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf(GREEN "background process %d was terminated by signal %d\n" RESET, pid, WTERMSIG(status));
        }
        else
        {
            printf(RED "background process %d did not exit normally\n" RESET, pid);
        }
    }
}

void whatCommand(char *command, int background)
{
    char func[1000], func_arg[1000];
    char *p = strstr(command, " ");
    if (p == NULL)
    {

        strcpy(func, command);

        bool b = execute_function_with_arg(func, func_arg);
        if (b)
        {
            return;
        }
    }
    else
    {
        int i = 0;
        while (command[i] != ' ')
        {
            func[i] = command[i];
            i++;
        }
        func[i] = '\0';

        bool b = execute_function_with_arg(func, p + 1);
        if (b)
        {
            return;
        }
    }



    char *temp = (char *)malloc(sizeof(char) * 1024);
    strcpy(temp, command);

    int size = strlen(command);

    if (size >= 3 && strncmp(command, "hop", 3) == 0)
    {
        hop(command);
    }
    else if (size >= 6 && strncmp(command, "reveal", 6) == 0)
    {
        reveal(command);
    }
    else if (size >= 3 && strncmp(command, "log", 3) == 0)
    {

        if (size == 3)
        {

            logShow();
        }
        else if (size >= 5 && command[4] == 'p' && command[5] == 'u' && command[6] == 'r')
        {
            logPurge();
        }
        else if (size >= 13 && command[4] == 'e' && command[5] == 'x' && command[6] == 'e')
        {
            logExecute(command);
        }
    }
    else if (size >= 8 && strncmp(command, "proclore", 8) == 0)
    {
        proclore(command);
    }
    else if (size >= 4 && strncmp(command, "seek", 4) == 0)
    {

        seek(command);
    }
    else if (size >= 10 && strncmp(command, "activities", 10) == 0)
    {
        activities();
    }
    else if (size >= 4 && strncmp(command, "ping", 4) == 0)
    {
        ping(command);
    }
    else if (size >= 2 && strncmp(command, "fg", 2) == 0)
    {
        fg(command);
    }
    else if (size >= 2 && strncmp(command, "bg", 2) == 0)
    {
        bg(command);
    }
    else if (size >= 7 && strncmp(command, "neonate", 7) == 0)
    {
        neonate(command);
    }
    else if (size >= 4 && strncmp(command, "iMan", 4) == 0)
    {
        iman(command);
    }
    else
    {
        // Set up the signal handler for SIGCHLD
        struct sigaction sa;
        sa.sa_handler = &handle_sigchld;
        sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
        sigaction(SIGCHLD, &sa, 0);

        char *arguments[MAX_ARG_COUNT];
        pid_t pid;
        int status;
        char *saveptr;
        int arg_count = 0;
        char *token = strtok_r(command, " ", &saveptr);
        while (token != NULL && arg_count < MAX_ARG_COUNT - 1)
        {
            if (token[0] == '"' || token[0] == '\'')
            {
                char quote_type = token[0];
                token++;
                char quoted_string[MAX_COMMAND_LENGTH] = {0};
                strcat(quoted_string, token);
                char *end_quote = strchr(quoted_string, quote_type);
                while (end_quote == NULL)
                {
                    char *next_token = strtok_r(NULL, " ", &saveptr);
                    if (next_token == NULL)
                    {
                        printf("Error: unmatched quote\n");
                        return;
                    }
                    strcat(quoted_string, " ");
                    strcat(quoted_string, next_token);
                    end_quote = strchr(quoted_string, quote_type);
                }

                *end_quote = '\0';
                arguments[arg_count++] = strdup(quoted_string);
            }
            else
            {
                arguments[arg_count++] = token;
            }

            token = strtok_r(NULL, " ", &saveptr);
        }

        arguments[arg_count] = NULL;

        if (background == 1)
        {

            pid = fork();

            if (pid < 0)
            {
                // Error occurred during fork
                perror("Fork failed");
                exit(1);
            }
            else if (pid == 0)
            {
                // Child process: execute the command
                setpgid(0, 0);
                if (execvp(arguments[0], arguments) < 0)
                {
                    perror(RED " Exec failed " RESET);
                    printf(RED "ERROR: Invalid Command\n" RESET);
                    exit(1);
                }
            }
            // setpgid(pid,pid);
            add_process(pid, arguments[0], 'b');
            printf("[PID:%d]\n", pid);
        }
        else
        {
            struct timeval start, end;
            int seconds;

            pid = fork();
            gettimeofday(&start, NULL);
            if (pid < 0)
            {
                // Error occurred during fork
                perror("Fork failed");
                exit(1);
            }
            else if (pid == 0)
            {
                // Child process: execute the command

                FOREGROUNDPID = getpid();
                if (execvp(arguments[0], arguments) < 0)
                {
                    perror(RED " Exec failed " RESET);
                    printf(RED "ERROR: Invalid Command\n" RESET);

                    exit(1);
                }
            }
            else
            {
                // Parent process: wait for the child process to finish
                add_process(pid, arguments[0], 'f');
                FOREGROUNDPID = pid;
                wait(&status);
                // printf("%d\n", FOREGROUNDPID);
                gettimeofday(&end, NULL);
                seconds = (int)(end.tv_sec - start.tv_sec);

                if (seconds > 2)
                {
                    strcpy(TWOSECMD, temp);
                }
            }
        }
    }
    free(temp);
}