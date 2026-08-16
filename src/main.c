#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell_header.h"

int main(int argc, char *argv[]){
	setbuf(stdout, NULL);
	char *input = malloc(INPUT_BUFFER_SIZE);
	while (1){
		printf("$ ");
		
		if (input == NULL){
		    printf("Memory allocation error.\n");
		    return 1;
		}
		fgets(input, INPUT_BUFFER_SIZE, stdin);
		input[strcspn(input, "\n")] = '\0';

		int params = 4;
		char *input_ptr, *token;
		int j = 0, input_tok_len = 0;
		char **input_tok = malloc(params*sizeof(char*));
		if (input_tok == NULL){
			return 1;
		}
		token = strtok_r(input, " ", &input_ptr);
		j++;
		input_tok[0] = token;
		while(token != NULL){
			token = strtok_r(NULL, " ", &input_ptr);
			input_tok_len++;
			input_tok[j] = token;
			j++;
			if (j >= params){
				char **temp = realloc(input_tok, 2*params*sizeof(char*));
				
				if (temp == NULL){
					free(input_tok);
					return 1;
				}
				input_tok = temp;
				params *= 2;
			}
		}
		if (input_tok_len < params){
			input_tok[input_tok_len] = NULL;
		}

		//printf("%s\n", command);
		int ind = search_cmd(input_tok[0]);
		if (ind == -1){
			if (launch_exec(input_tok, input_tok_len) == -1){
				printf("%s: not found\n", input_tok[0]);
			}

		}
		else{
			commands[ind].func(input_tok, input_tok_len);
		}
		//(ind == -1) ? printf("%s: command not found\n", input_tok[0]) : commands[ind].func(input_tok, input_tok_len);
		
	}
}

/*
		char *inp_ptr = input;
		while (*inp_ptr == ' ') {inp_ptr++;}
		char command[50];
		int j = 0;
		while (j < strcspn(inp_ptr, " ")) {
			command[j] = inp_ptr[j];
			j++;
		}
		command[j] = '\0';

*/