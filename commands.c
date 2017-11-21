#include "commands.h"

#include<string.h>
#include<unistd.h>
#include<stdio.h>

builtin builtin_funcs[] = {
	{"help", "", help_func},
	{"pwd", "Prints current working directory", pwd_func}
};

int builtin_commands = sizeof(builtin_funcs)/sizeof(builtin);

void pwd_func(char** args){
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("%s\n", cwd);
}

void help_func(char** args){
	int i;
	printf("\nFollowing functions have been implemented in krashell: \n");
	for(i=0;i<sizeof(builtin_funcs)/sizeof(builtin);i++){
		if(strcmp(builtin_funcs[i].description,"") != 0){
			printf("\t%s: %s\n", builtin_funcs[i].name, builtin_funcs[i].description);
		}
	}
	printf("\n");
}


