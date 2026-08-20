#include "shell_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>
#include <ctype.h>

//const char double_quotes_escape_chars[ESCAPE_CHARS_LEN] = {'\\','\"','$','`'};

static const unsigned char char_lookup[256] = {
	[' '] = 1, ['\t'] = 1, ['\n'] = 1,
	['\\'] = 2, ['\"'] = 2, ['$'] = 2, ['`'] = 2
};

const builtin_command commands[CMD_CNT] = {
	{&(echo), "echo"},
	{&(exit_shell),"exit"},
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
	for (int i = 0; i < len; ++i){
		free(argv[i]);
	}
	free(argv);
	exit(EXIT_SUCCESS);
}
void echo(char **argv, int len){
	for (int i = 1; i < len && argv[i] != NULL; ++i){
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

	int ind = search_cmd(argv[1]);
	if (ind == -1) {
		char *full_path = search_exec(argv[1], strlen(argv[1]));
		if (full_path != NULL) {
			printf("%s is %s\n", argv[1], full_path);
			free(full_path);
		}
		else {
			printf("%s: not found\n", argv[1]);
		}
		
	}
	else {
		printf("%s is a shell builtin\n", argv[1]);
	}
		
}
int launch_exec(char **argv, int len){
	char *full_path = search_exec(argv[0], strlen(argv[0]));
	if (full_path == NULL){
		return -1;
	}
	
	pid_t id = fork();
	if (id > 0){
		int status;
        waitpid(id, &status, 0); 
		free(full_path);
		return 0;
	}
	else if (id == 0){
		execv(full_path, argv);		
	}
	else{
		free(argv);
		free(full_path);
		printf("bruh");
	}
		
}
static void create_token(char ***input_tok, int *size, int *len, char token_buffer[], int *token_buffer_len){
	token_buffer[*token_buffer_len] = '\0';
	if (*len >= *size){
		char **temp = realloc(*input_tok, (*size)*2*sizeof(char*));
		if (temp == NULL){
			fprintf(stderr, "Memory allocation error.\n");
			return;
		}
		(*size) *= 2;
		(*input_tok) = temp;
	}
	(*input_tok)[*len] = strdup(token_buffer);
	*token_buffer_len = 0;
	(*len)++;
}
void parse_input(char *input, char ***input_tok, int *size, int *len){
	char c;
	char token_buffer[INPUT_BUFFER_SIZE] = {0};
	parse_states state = NORMAL;
	int j = 0, token_buffer_len = 0;
	*len = 0;
	while ((c = input[j]) != '\0'){
		
		switch (state){
			case NORMAL:
	
				if (char_lookup[(unsigned char)c] & 1){
					if (token_buffer_len != 0){
						create_token(input_tok, size, len, token_buffer, &token_buffer_len);
					}
				}
				else if (c == '\\'){
					state = NORMAL_ESC;
				}
				else if (c == '\''){
					state = SINGLE_QUOTE;
				}
				else if (c == '\"'){
					state = DOUBLE_QUOTE;
				}
				else{
					token_buffer[token_buffer_len++] = c;
				}

			break;
			case NORMAL_ESC:
				token_buffer[token_buffer_len++] = c;
				state = NORMAL;
			break;
			case SINGLE_QUOTE:
				if (c == '\''){
					if (input[j+1] == '\'' || input[j-1] == '\''){
						state = NORMAL;
					}
					else {
						create_token(input_tok, size, len, token_buffer, &token_buffer_len);
						state = NORMAL;
					}
				}
				else{
					token_buffer[token_buffer_len] = c;
					token_buffer_len++;
				}
			break;
			case DOUBLE_QUOTE:

				if (c == '\"'){
					if (input[j+1] == '\"' || input[j-1] == '\"' || !isspace(input[j+1])){
						state = NORMAL;
					}
					else {
						create_token(input_tok, size, len, token_buffer, &token_buffer_len);
						state = NORMAL;
					}
				}
				else if (c == '\\'){
					state = DOUBLE_QUOTE_ESC;
				}
				else{
					token_buffer[token_buffer_len++] = c;
				}

			break;
			case DOUBLE_QUOTE_ESC:
				if (char_lookup[(unsigned char)c] > 0){
					token_buffer[token_buffer_len++] = c;
				}
				state = DOUBLE_QUOTE;
			break;
		}
		
		j++;
	}
	if (token_buffer_len > 0){
		create_token(input_tok, size, len, token_buffer, &token_buffer_len);
	}

}
void redirect_stdout(char **argv, int len){

}

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

