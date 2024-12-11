#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#include <stdbool.h>
#include "global.h"
#include "hop.h"
#include "color.h"

void helper(const char *base_dir, const char *search_term, const char *target_dir, char flag, bool e, int *fileCount, int *dirCount, char *found_file, char *found_dir)
{
    DIR *dir;
    struct dirent *entry;
    struct stat path_stat;
    char path[1024];

    if (!(dir = opendir(base_dir)))
    {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue; // Skip "." and ".."
        }

        snprintf(path, sizeof(path), "%s/%s", base_dir, entry->d_name);
        stat(path, &path_stat);

        // Calculate the relative path from the target directory
        const char *relative_path = path + strlen(target_dir) + 1;

        if (S_ISDIR(path_stat.st_mode))
        {
            // Directory found, print in blue
            if (strstr(entry->d_name, search_term) && (flag == 'd' || flag == 'x'))
            {
                (*dirCount)++;
                strncpy(found_dir, path, sizeof(path));
                printf(BLUE "%s\n" RESET, relative_path);
            }
            // Recursively search in this directory
            helper(path, search_term, target_dir, flag, e, fileCount, dirCount, found_file, found_dir);
        }
        else if (S_ISREG(path_stat.st_mode))
        {
            // File found, print in green
            if (strstr(entry->d_name, search_term) && (flag == 'f' || flag == 'x'))
            {
                (*fileCount)++;
                strncpy(found_file, path, sizeof(path));
                printf(GREEN "%s\n" RESET, relative_path);
            }
        }
    }

    closedir(dir);
}

void seek(char *command)
{

    int fileCount = 0, dirCount = 0;
    char found_file[1024] = {0};
    char found_dir[1024] = {0};

    char *saveptr;

    char *token = strtok_r(command, " ", &saveptr);

    char *path = malloc(sizeof(char) * 1024);
    strcpy(path, ".");
    char *search = malloc(sizeof(char) * 1024);
    strcpy(search, "");
    char *flags = malloc(sizeof(char) * 10);
    strcpy(flags, "");

    int count = 0;
    while (token != NULL)
    {
        if (strcmp(token, "seek") == 0)
        {
        }
        else if (token[0] == '-')
        {
            strcpy(flags + strlen(flags), token + 1);
        }
        else
        {
            count++;
            if (count == 1)
            {
                strcpy(search, token);
            }
            else if (count == 2)
            {
                strcpy(path, token);
            }
        }
        token = strtok_r(NULL, " ", &saveptr);
    }

    if (strcmp(path, "~") == 0)
    {
        strcpy(path, HOME_DIR);
    }

    char current_path[1024];
    if (realpath(path, current_path) == NULL)
    {
        perror("realpath");
        return;
    }

    bool hasF = (strstr(flags, "f")) ? true : false;
    bool hasD = (strstr(flags, "d")) ? true : false;
    bool hasE = (strstr(flags, "e")) ? true : false;

    if (hasF && hasD)
    {
        printf("ERROR: Invalid flags!!!\n");
        return;
    }

    if (!hasF & !hasD)
    {
        helper(current_path, search, current_path, 'x', true, &fileCount, &dirCount, found_file, found_dir);
    }

    if (hasF)
    {
        helper(current_path, search, current_path, 'f', true, &fileCount, &dirCount, found_file, found_dir);
    }
    if (hasD)
    {
        helper(current_path, search, current_path, 'd', true, &fileCount, &dirCount, found_file, found_dir);
    }

    if (fileCount == 0 && dirCount == 0)
    {
        printf(RED "No match found\n" RESET);
    }

 
    if (hasE)
    {
        FILE *file;
        char *buffer;
        if (fileCount > 0)
        {

            file = fopen(found_file, "r");
            if (file == NULL)
            {
                perror("Error opening file");
                return;
            }
            fseek(file, 0, SEEK_END);
            long file_size = ftell(file);
            rewind(file);

            buffer = (char *)malloc(file_size + 1);
            if (buffer == NULL)
            {
                perror("Memory error");
                fclose(file);
                return;
            }

            fread(buffer, 1, file_size, file);

            buffer[file_size] = '\0';
        }

        if (hasD && dirCount == 1)
        {
            // printf("%s\n", found_dir);

            if (chdir(found_dir) == 0)
            {
                getcwd(CUR_DIR, sizeof(CUR_DIR));
            }
            else
            {
                perror("cant change dir\n");
            }
        }
        else if (hasF && fileCount == 1)
        {
            printf("%s\n", buffer);
        }
        else
        {
            if (dirCount == 1 && fileCount == 0)
            {
                // printf("%s\n", found_dir);
                if (chdir(found_dir) == 0)
                {
                    getcwd(CUR_DIR, sizeof(CUR_DIR));
                }
                else
                {
                    perror("cant change dir\n");
                }
            }

            if (dirCount == 0 && fileCount == 1)
            {
                printf("%s\n", buffer);
            }
        }
        if (fileCount > 0){
            fclose(file);
            free(buffer);
        }
    }
}