#include "reveal.h"
#include "hop.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include "color.h"

#define COLOR_GREEN "\033[0;32m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_RESET "\033[0m"

// Correct compare function for scandir
int compare(const struct dirent **a, const struct dirent **b)
{
  return strcmp((*a)->d_name, (*b)->d_name);
}

void print_file_info(const char *path, struct dirent *de)
{
  struct stat file_stat;
  char fullPath[1024];

  snprintf(fullPath, sizeof(fullPath), "%s/%s", path, de->d_name);

  if (stat(fullPath, &file_stat) == -1)
  {
    perror(RED "stat" RESET);
    return;
  }

  // Print file type and permissions
  printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
  printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
  printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
  printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
  printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
  printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
  printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
  printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
  printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
  printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");

  // Print number of hard links
  printf(" %ld", (long)file_stat.st_nlink);

  // Print owner and group names
  struct passwd *pw = getpwuid(file_stat.st_uid);
  struct group *gr = getgrgid(file_stat.st_gid);
  printf(" %s %s", pw->pw_name, gr->gr_name);

  // Print file size
  printf(" %ld", (long)file_stat.st_size);

  // Print last modification time
  char time_str[20];
  struct tm *tm_info = localtime(&file_stat.st_mtime);
  strftime(time_str, sizeof(time_str), "%b %d %H:%M", tm_info);
  printf(" %s", time_str);

  // Print the file name with color based on type
  if (S_ISDIR(file_stat.st_mode))
  {
    printf(" " COLOR_BLUE "%s" COLOR_RESET "\n", de->d_name);
  }
  else if (file_stat.st_mode & S_IXUSR)
  {
    printf(" " COLOR_GREEN "%s" COLOR_RESET "\n", de->d_name);
  }
  else
  {
    printf(" %s\n", de->d_name);
  }
}

void show_folder(const char *path, bool hasA)
{
  struct dirent **namelist;
  int n;

  // Scan directory entries and store them in namelist
  n = scandir(path, &namelist, NULL, compare);
  if (n < 0)
  {
    perror(RED "scandir" RESET);
    return;
  }

  // Iterate over the sorted entries and print file info
  for (int i = 0; i < n; i++)
  {
    char full_path[1024];
    struct stat file_stat;

    snprintf(full_path, sizeof(full_path), "%s/%s", path, namelist[i]->d_name);

    if (stat(full_path, &file_stat) == -1)
    {
      perror(RED "stat" RESET);
      free(namelist[i]);
      continue;
    }

    if (hasA || namelist[i]->d_name[0] != '.')
    {
      if (S_ISDIR(file_stat.st_mode))
      {
        printf(COLOR_BLUE "%s" COLOR_RESET "\n", namelist[i]->d_name);
      }
      else if (file_stat.st_mode & S_IXUSR)
      {
        printf(COLOR_GREEN "%s" COLOR_RESET "\n", namelist[i]->d_name);
      }
      else
      {
        printf("%s\n", namelist[i]->d_name);
      }
    }

    free(namelist[i]);
  }
  free(namelist);
}

void show_folder_details(const char *path, bool hasA)
{
  struct dirent **namelist;
  int n;

  // Scan directory entries and store them in namelist
  n = scandir(path, &namelist, NULL, compare);
  if (n < 0)
  {
    perror(RED "scandir" RESET);
    return;
  }

  // Iterate over the sorted entries and print file info
  for (int i = 0; i < n; i++)
  {
    if (hasA)
    {
      print_file_info(path, namelist[i]);
    }
    else
    {
      if (namelist[i]->d_name[0] != '.')
      {
        print_file_info(path, namelist[i]);
      }
    }

    free(namelist[i]);
  }
  free(namelist);
}

void reveal(char *revealCommand)
{
  char *buffer = (char *)malloc(sizeof(char) * BUF_SIZE);
  if (buffer == NULL)
  {
    perror(RED "malloc" RESET);
    return;
  }
  strcpy(buffer, revealCommand);

  char *flags = (char *)malloc(sizeof(char) * BUF_SIZE);
  if (flags == NULL)
  {
    perror(RED "malloc" RESET);
    free(buffer);
    return;
  }
  strcpy(flags, "");

  char *path = (char *)malloc(sizeof(char) * BUF_SIZE);
  if (path == NULL)
  {
    perror(RED "malloc" RESET);
    free(buffer);
    free(flags);
    return;
  }
  strcpy(path, "");

  char *token = strtok(buffer, " ");
  while (token != NULL)
  {
    if (strcmp(token, "reveal") == 0)
    {
      // Skip the "reveal" command
    }
    else if (token[0] == '-' && strlen(token) > 1)
    {
      strcpy(flags + strlen(flags), token + 1);
    }
    else
    {
      strcpy(path, token);
    }
    token = strtok(NULL, " ");
  }

  if (strlen(path) == 0 || strcmp(path, ".") == 0)
  {
    strcpy(path, CUR_DIR);
  }
  else if (strcmp(path, "..") == 0)
  {
    if (chdir("..") == 0)
    {
      getcwd(path, BUF_SIZE);
    }
    else
    {
      perror(RED "cant change dir to ..\n" RESET);
    }

    if (chdir(CUR_DIR) == 0)
    {
    }
    else
    {
      perror(RED "cant change dir back\n" RESET);
    }
  }
  else if (strcmp(path, "~") == 0)
  {

    strcpy(path, HOME_DIR);
  }
  else if (strcmp(path, "-") == 0)
  {

    strcpy(path, PREV_DIR);
  }
  else
  {
    if (chdir(path) == 0)
    {
      getcwd(path, BUF_SIZE);
    }
    else
    {
      perror(RED "cant change dir to path\n" RESET);
    }

    if (chdir(CUR_DIR) == 0)
    {
    }
    else
    {
      perror(RED "cant change dir back\n" RESET);
    }
  }

  bool hasL = strstr(flags, "l") ? true : false;
  bool hasA = strstr(flags, "a") ? true : false;

  if (hasL)
  {
    if (hasA)
    {
      show_folder_details(path, hasA);
    }
    else
    {
      show_folder_details(path, hasA);
    }
  }
  else
  {
    if (hasA)
    {
      show_folder(path, hasA);
    }
    else
    {
      show_folder(path, hasA);
    }
  }

  free(buffer);
  free(flags);
  free(path);
}