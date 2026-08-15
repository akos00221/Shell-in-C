#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell_header.h"

int main(int argc, char *argv[]){
	setbuf(stdout, NULL);
	
	while (1){
		printf("$ ");
		char *input = malloc(INPUT_BUFFER_SIZE);
		if (input == NULL){
		    printf("Memory allocation error.\n");
		    return 1;
		}
		fgets(input, INPUT_BUFFER_SIZE, stdin);
		input[strcspn(input, "\n")] = '\0';
		char *inp_ptr = input;
		while (*inp_ptr == ' ') {inp_ptr++;}
		char command[50];
		int j = 0;
		while (j < strcspn(inp_ptr, " ")) {
			command[j] = inp_ptr[j];
			j++;
		}
		command[j] = '\0';
		//printf("%s\n", command);
		int ind = search_cmd(command);
		(ind == -1) ? printf("%s: command not found\n", command) : commands[ind].func(inp_ptr, (int)strnlen(inp_ptr, sizeof(input)));
	}
}