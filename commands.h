#ifndef COMMANDS_H_INCLUDED
#define UTILS_H_INCLUDED

typedef struct builtin{
	const char* name;
	const char* description;
	void (*func)(char** args);
}builtin;
extern int builtin_commands;
void pwd_func(char** args);
void help_func(char** args);
extern builtin builtin_funcs[];
#endif
