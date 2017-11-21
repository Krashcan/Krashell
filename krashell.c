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
void exec_if_builtin(char* cmd, char** args);

typedef struct builtin{
	const char* name;
	const char* description;
	void (*func)(char** args);
}builtin;

short int builtin_sorted = 0;

builtin builtin_funcs[] = {
	{"pwd", "Prints current working directory", pwd_func},
	{"help", "Prints"}};

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
		pid_t pid, wpid;
		int status = 0;
		pid = fork();
		if(pid==0){
			char* words[50];
			char* args[50];
			split(cmd, ' ', words);
			int i;
			for(i=0;strcmp(words[i], "EOF\0");i++){
				if(i==0){
					strcpy(cmd, words[i]);
				}
				else{
					args[i-1] = (char*)malloc(sizeof(words[i]));
					strcpy(args[i-1], words[i]);
				}
			}
			exec_if_builtin(cmd, args);
			exit(0);
		}else if(pid < 0){
			printf("Error occurred in creating child process \n");
			exit(1);
		}
		while((wpid = wait(&status)) > 0);
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
	for(i=0;str[i]!='\n';i++){
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
		words[j] = (char*)malloc((i - last)*sizeof(char));
		strncpy(words[j], str+last, sizeof(words[j])-1);
		words[j][i-last] = '\0';
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
	//for(i=0;i
}
