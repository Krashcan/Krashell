#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>

void startup();
void split(char* str, char delim, char* words[50]);
int builtin_compare(const void* a, const void* b);
int cmd_exists(char* key, int l, int r);
void pwd_func(char** args);
void help_func(char** args);
void exec_if_builtin(char* cmd, char** args);

typedef struct builtin{
	const char* name;
	const char* description;
	void (*func)(char** args);
}builtin;

short int builtin_sorted = 0;

builtin builtin_funcs[] = {
	{"pwd", "Prints current working directory", pwd_func},
	{"help", "", help_func}};

int main(int argc, char* argv[]){
	system("clear");
	startup();
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
			pid_t pid = wait(&status);
		}
	}

	printf("Krashell is terminating...\n");
	return 0;
}

void startup(){
	if(!builtin_sorted)
		qsort(builtin_funcs, sizeof(builtin_funcs)/sizeof(builtin), sizeof(builtin), builtin_compare);
	builtin_sorted = 1;

}

int builtin_compare(const void* a, const void* b){
	return strcmp((*(builtin*)a).name, (*(builtin*)b).name);
}

void split(char* str, char delim, char* words[50]){
	int i, j=0, last=0;
	for(i=0;str[i]!='\0';i++){
		if(str[i] == delim){
			if(last == i){
				last = i + 1;
				continue;
			}
			words[j] = (char*)malloc((i - last + 1)*sizeof(char));
			strncpy(words[j], str+last, sizeof(words[j])-1);
			words[j][i-last] = '\0';
			last = i + 1;
			j++;
		}		
	}
	if(last != i){
		words[j] = (char*)malloc((i - last));
		strncpy(words[j], str+last, i-last);
		if(words[j][i-last-1] == '\n'){
			words[j][i-last-1] = '\0';
		}else{
			words[j][i-last] = '\0';
		}
		words[j+1] = "EOF";
	}
}

int cmd_exists(char* key, int l, int r){
	if(l<=r){
		int m = (l + r)/2 ;
		if(strcmp(builtin_funcs[m].name, key) == 0) 
			return m;
		else if(strcmp(builtin_funcs[m].name, key) < 0)
			return cmd_exists(key, m+1, r);
		return cmd_exists(key, l, m-1);
	}
	return -1;
}

void exec_if_builtin(char* cmd, char** args){
	int cmd_num = cmd_exists(cmd, 0, sizeof(builtin_funcs)/sizeof(builtin)-1);
	if(cmd_num != -1){
		builtin_funcs[cmd_num].func(args);
	}else{
		printf("%s: Command not found.\n(Hint: Try 'help' for list of commands)\n", cmd);
	}
}

void pwd_func(char** args){
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("%s\n", cwd);
}

void help_func(char** args){
	int i;
	printf("\nFollowing functions have been implemented in krashell: \n");
	for(i=0;i<sizeof(builtin_funcs)/sizeof(builtin);i++){
		if(builtin_funcs[i].description != ""){
			printf("\t%s: %s\n", builtin_funcs[i].name, builtin_funcs[i].description);
		}
	}
	printf("\n");
}
