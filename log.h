#ifndef LOG_H
#define LOG_H

#define MAX_COMMANDS 15
#define MAX_COMMAND_LENGTH 1024
#define LOG_FILE "command_log.txt"

// Structure to hold a command
typedef struct {
    char command[MAX_COMMAND_LENGTH];
} Command;

void add_to_log(const char *cmd);
void load_log();
void save_log();
void logShow();
void logPurge();
void logExecute(char * command);


#endif
