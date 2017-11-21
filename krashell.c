#include "utils.h"
#include "commands.h"

#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>


int main(int argc, char* argv[]){
	system("clear");
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	const char* prompt = "guest@krashell:~";
	char *cmd = NULL;
	size_t bufsize;
	while(1){
		printf("%s%s$ ", prompt, cwd);
		getline(&cmd, &bufsize, stdin);
		char* commands[50];
		split(cmd, ';', commands);
		int i, j;
		
		for(i=0;strcmp(commands[i], "EOF\0");i++){
			pid_t pid = fork();
			if(pid < 0){
				printf("Error forking child.\n");
				exit(1);
			}else if(pid == 0){
				char *words[50], *args[50];
				split(commands[i], ' ', words);
				for(j=0;strcmp(words[j], "EOF\0");j++){
					if(j==0){
						strcpy(cmd, words[j]);
					}else{
						args[j-1] = (char*)malloc(sizeof(words[j]));
						strcpy(args[j-1], words[j]);
					}
				}
				exec_if_builtin(cmd, args);
				exit(0);
			}
		}
		for(i=0;strcmp(commands[i], "EOF\0");i++){
			int status;
			wait(&status);
		}
	}

	printf("Krashell is terminating...\n");
	return 0;
}
