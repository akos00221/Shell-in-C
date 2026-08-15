#include "shell_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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
void type(char *argv, int len){
    char *input = argv;
    char *builtin_function_parameter = input+5;
	int ind = search_cmd(builtin_function_parameter);
	(ind == -1) ? printf("%s: not found\n", builtin_function_parameter) : printf("%s is a shell builtin\n", builtin_function_parameter);
}
void redirect_stdout(char *argv, int len){}

