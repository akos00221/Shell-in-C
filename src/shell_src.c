#include "shell_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>

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

void exit_shell(char **argv, int len){
	free(argv);
	exit(EXIT_SUCCESS);
}
void echo(char **argv, int len){
	for (int i = 1; i < len; ++i){
		printf("%s ", argv[i]);
	}
	printf("\n");
}

static char *search_exec(char *argv, int len){
    const char *env = getenv("PATH");
	if (env == NULL){
		fprintf(stderr, "No path variables.\n");
		return NULL;
	}
	char *env_cpy = strdup(env);
	if (env_cpy == NULL){
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}
	
	char *token, *saveptr;
	token = strtok_r(env_cpy, ":", &saveptr);
	
	while (token != NULL){
		int token_len = strlen(token);
		char *full_path = malloc(token_len+len+2);
		if (full_path == NULL){
			fprintf(stderr, "Memory allocation error.\n");
			return NULL;
		}
		sprintf(full_path, "%s/%s", token, argv);
		//printf("%s\n", full_path);
		if (!access(full_path, X_OK)){
			free(env_cpy);
			return full_path;
		}
		//free(full_path);
		token = strtok_r(NULL, ":", &saveptr);
	}
	free(env_cpy);
	return NULL;
}

void type(char **argv, int len){

	int ind = search_cmd(argv[0]);
	if (ind == -1) {
		char *full_path = search_exec(argv[0], strlen(argv[0]));
		if (full_path != NULL) {
			printf("%s is %s\n", argv[0], full_path);
			free(full_path);
		}
		else {
			printf("%s: not found\n", argv[0]);
		}
		
	}
	else {
		printf("%s is a shell builtin\n", argv[0]);
	}
		
}
int launch_exec(char **argv, int len){
	char *full_path = search_exec(argv[0], strlen(argv[0]));
	if (full_path == NULL){
		return -1;
	}
	
	pid_t id = fork();
	if (id > 0){
		printf("\n");
		execv(full_path, argv);
		
		return 0;
	}
	else if (id == 0){
		int status;
        waitpid(id, &status, 0); 
		free(full_path);
		return 0;
	}
	else{
		free(argv);
		free(full_path);
		printf("bruh");
	}
		
}

void redirect_stdout(char **argv, int len){}



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