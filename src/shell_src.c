#include "shell_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

const builtin_command commands[CMD_CNT] = {
	{&(echo), "echo"},
	{&(exit_shell),"exit"},
	{&(redirect_stdout), ">"},
	{&(type), "type"}
	
};

int search_cmd(char *key){
	int high = CMD_CNT - 1;
	int low = 0;
	while (low <= high){
		int mid = low + (high - low) / 2;
		if (!strcmp(commands[mid].name, key)) return mid;
		else if (strcmp(commands[mid].name, key) < 0) low = ++mid;
		else {high = --mid;}
	}
	return -1;
}

void exit_shell(char *argv, int len){
	exit(EXIT_SUCCESS);
}
void echo(char *argv, int len){
	char *input = argv;
	char *builtin_function_parameter = input+5;
	printf("%s\n", builtin_function_parameter);
}

static int search_exec(char *argv, int len){
    const char *env = getenv("PATH");
	char env_cpy[strlen(env) + 1];
	strcpy(env_cpy, env);
	char *token, *saveptr;
	token = strtok_r(env_cpy, ":", &saveptr);
	
	while (token != NULL){
		char full_path[strlen(token)+len];
		strcpy(full_path, token);
		strcat(full_path, argv);
		if (!access(full_path, X_OK)){
			return 0;
		}
		token = strtok_r(NULL, ":", &saveptr);
	}
	return 1;
}

void type(char *argv, int len){
	char *input = argv;
    char *builtin_function_parameter = input+5;
	int ind = search_cmd(builtin_function_parameter);
	(ind == -1) ? printf("%s: not found\n", builtin_function_parameter) : printf("%s is a shell builtin\n", builtin_function_parameter);
}
void redirect_stdout(char *argv, int len){}

/*
		DIR *dir = opendir(token);
		while (dir != NULL){
			if (access())
			
			struct dirent *entry = readdir(dir);
			while (entry != NULL){
				if (!strcmp(argv, entry->d_name) && access()){
					
				}
			}
			
			closedir(dir);
		}
*/