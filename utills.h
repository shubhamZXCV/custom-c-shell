#ifndef UTILS_H
#define UTILS_H

int count_char_occurrences(const char *str, char target);

void trim_extra_spaces(char *str);

void extract_redirections(const char *input, char *outputFile, char *inputFile, char *appendFile);

#endif