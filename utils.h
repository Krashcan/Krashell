#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

void split(char* str, char delim, char* words[50]);
int builtin_compare(const void* a, const void* b);
int cmd_exists(char* key, int l, int r);
void exec_if_builtin(char* cmd, char** args);

#endif
