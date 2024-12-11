// alias.h
#include<stdbool.h>
#ifndef ALIAS_H
#define ALIAS_H

#define MAX_LINE_LENGTH 1024



typedef struct Alias {
    char *key;
    char *value;
} Alias;

extern Alias *aliases;    
extern int alias_count;

typedef struct Function{
    char * name;
    int com_seq_size;
    char* com_seq[];
}Fuction;
void load_aliases();

int find_alias(const char *key);   
char *replace_aliases_in_string(const char *input);  
bool execute_function_with_arg(const char *function_name, const char *arg) ;

#endif 