#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utills.h"
#include "handleInput.h"
#include <stdbool.h>
#include "command.h"
#include "global.h"

Alias *aliases = NULL;
int alias_count = 0;
int find_alias(const char *key)
{
    for (int i = 0; i < alias_count; i++)
    {
        if (strcmp(aliases[i].key, key) == 0)
        {
            return i; // Return the index of the found alias
        }
    }
    return -1; // Return -1 if not found
}

void load_aliases()
{
    FILE *file;
    char line[MAX_LINE_LENGTH];

    file = fopen(".myshrc", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    while (fgets(line, sizeof(line), file))
    {

        if (strncmp(line, "alias ", 6) == 0)
        {
            char *key_start, *value_start, *key_end, *value_end;
            char *temp = line + 6;

            key_start = temp;
            key_end = strchr(temp, '=');
            if (key_end == NULL)
                continue;

            *key_end = '\0';

            value_start = key_end + 1;
            value_end = value_start + strlen(value_start) - 1;

            while (*value_start == ' ')
                value_start++;
            while (value_end > value_start && (*value_end == ' ' || *value_end == '\n'))
                value_end--;
            *(value_end + 1) = '\0';

            aliases = realloc(aliases, (alias_count + 1) * sizeof(Alias));
            trim_extra_spaces(key_start);
            trim_extra_spaces(value_start);
            aliases[alias_count].key = strdup(key_start);
            aliases[alias_count].value = strdup(value_start);

            alias_count++;
        }
    }
    fclose(file);

    // for (int i = 0; i < alias_count; i++) {
    //     printf("Alias %d: key='%s', value='%s'\n", i + 1, aliases[i].key, aliases[i].value);

    // }
}

bool execute_function_with_arg(const char *function_name, const char *arg)
{
    char CONFIGPATH[1024];
    strcpy(CONFIGPATH, HOME_DIR);
    strcat(CONFIGPATH, "/.myshrc");

    FILE *file = fopen(CONFIGPATH, "r");
    if (file == NULL)
    {
        perror("Failed to open .myshrc");
        return false;
    }

    char line[MAX_LINE_LENGTH];
    int in_function = 0;
    bool b = false;
    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        line[strcspn(line, "\n")] = 0;
        if (strncmp(function_name, line, strlen(function_name)) == 0 &&
            strstr(line, "()") != NULL && line[strlen(function_name)] == '(')
        {
            in_function = 1;
            b = true;
            continue;
        }
        if (in_function)
        {
            if (strstr(line, "{"))
            {
                continue;
            }

            if (strchr(line, '}'))
            {
                break;
            }
            char *placeholder = strstr(line, "$1");
            if (placeholder != NULL)
            {
                char modified_line[MAX_LINE_LENGTH];

                strncpy(modified_line, line, placeholder - line - 1);
                modified_line[placeholder - line - 1] = '\0';
                strcat(modified_line, arg);
                strcat(modified_line, placeholder + 3);

                modified_line[strlen(modified_line) - 1] = '\0';

                whatCommand(modified_line, 0);
            }
            else
            {
                whatCommand(line, 0);
            }
        }
    }
    fclose(file);
    return b;
}

char *replace_aliases_in_string(const char *input)
{
    if (input == NULL)
    {
        fprintf(stderr, "Error: input string is NULL.\n");
        return NULL;
    }

    // Start with a copy of the original input
    char *result = strdup(input);
    if (result == NULL)
    {
        perror("Error duplicating input string");
        return NULL;
    }

    for (int i = 0; i < alias_count; i++)
    {
        if (aliases[i].key == NULL || aliases[i].value == NULL)
        {
            fprintf(stderr, "Error: aliases[%d].key or aliases[%d].value is NULL.\n", i, i);
            free(result);
            return NULL;
        }

        char *pos;
        while ((pos = strstr(result, aliases[i].key)) != NULL)
        {
            // Calculate lengths of the strings
            size_t len_before = pos - result;
            size_t len_key = strlen(aliases[i].key);
            size_t len_value = strlen(aliases[i].value);
            size_t len_after = strlen(pos + len_key);

            // Allocate a new string with enough space for replacement
            char *new_result = malloc(len_before + len_value + len_after + 1);
            if (new_result == NULL)
            {
                perror("Error allocating memory for replacement");
                free(result);
                return NULL;
            }

            // Construct the new string
            strncpy(new_result, result, len_before);                    // Copy the part before the key
            strcpy(new_result + len_before, aliases[i].value);          // Replace the key with value
            strcpy(new_result + len_before + len_value, pos + len_key); // Copy the part after the key

            free(result);
            result = new_result;
        }
    }

    return result;
}
