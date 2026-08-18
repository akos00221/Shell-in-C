#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell_header.h"

int main(int argc, char *argv[]){
	setbuf(stdout, NULL);
	char input[INPUT_BUFFER_SIZE];
	while (1){
		printf("$ ");
		fgets(input, INPUT_BUFFER_SIZE-1, stdin);
		input[strcspn(input, "\n")] = '\0';

		int params = 4;

		int input_tok_len = 0;
		char **input_tok = malloc(params*sizeof(char*));
		if (input_tok == NULL){
			return 1;
		}
		parse_input(input, input_tok, &params, &input_tok_len);

		if (input_tok_len < params){
			input_tok[input_tok_len] = NULL;
		}
        //for (int i = 0; i < input_tok_len; ++i){
        //    printf("%s %d\n", input_tok[i], strlen(input_tok[i]));
        //}
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
		for (int i = 0; i < input_tok_len; ++i){
			free(input_tok[i]);
		}
		free(input_tok);
		
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