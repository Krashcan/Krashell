#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>

void split(char* str, char delim, char* words[50]);
//void evaluate_command(char* cmd);
/*
struct builtin{
	const char* name;
	int (*func)(const char** args);
};

builtin builtin_funcs[] = {
	{"ls", ls_func}, {"cat", cat_func}, {"cd", cd_func},
	{"pwd", pwd_func}, {"mkdir", mkdir_func}}
*/
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
		pid_t pid, wpid;
		int status = 0;
		pid = fork();
		if(pid==0){
			char* w[50];
			split(cmd, ' ', w);
			int i;
			for(i=0;strcmp(w[i], "EOF\0");i++){
				printf("%s\n",w[i]);
			}
			//evaluate_command(cmd);
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

void split(char* str, char delim, char* words[50]){
	int i, j=0, last=0;
	for(i=0;str[i]!='\0';i++){
		if(str[i] == delim){
			if(last == i) continue;
			words[j] = (char*)malloc((i - last + 1)*sizeof(char));
			strncpy(words[j], str+last, i);
			words[j][i-last] = '\0';
			last = i + 1;
			j++;
		}		
	}
	words[j] = (char*)malloc((i - last)*sizeof(char));
	strncpy(words[j], str+last, i+1);
	words[j+1] = "EOF";
}
/*
void load_commands(){
	// ls command to list directory files and folders
	commands[0].cmd_str = "ls";
	commands[0].cmd_ptr = list_directory;
}

int exec_if_built_in(char* instructions[50]){
	
}
*/
