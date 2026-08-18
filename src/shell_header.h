#ifndef SHELL_H
#define SHELL_H

#define CMD_CNT 4
#define INPUT_BUFFER_SIZE 4096
#define ESCAPE_CHARS_LEN 4

extern const char double_quotes_escape_chars[ESCAPE_CHARS_LEN];

typedef enum {
	NORMAL,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}parse_states;
typedef enum {
	OFF,
	ON
}escape_mode;

typedef void (*command_function)(char**, int);
typedef struct {
	command_function func;
	char *name;
}builtin_command;

int search_cmd(char *key);
void exit_shell(char **argv, int len);
void echo(char **argv, int len);
void type(char **argv, int len);
void redirect_stdout(char **argv, int len);
int launch_exec(char **argv, int len);

void parse_input(char *input, char **input_tok, int *size, int *len);

extern const builtin_command commands[CMD_CNT];

#endif