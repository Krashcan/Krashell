#include "utils.h"
#include "commands.h"

#include<stdlib.h>
#include<string.h>
#include<stdio.h>


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


int builtin_compare(const void* a, const void* b){
	return strcmp((*(builtin*)a).name, (*(builtin*)b).name);
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
	int cmd_num = cmd_exists(cmd, 0, builtin_commands-1);
	if(cmd_num != -1){
		builtin_funcs[cmd_num].func(args);
	}else{
		printf("%s: Command not found.\n(Hint: Try 'help' for list of commands)\n", cmd);
	}
}


