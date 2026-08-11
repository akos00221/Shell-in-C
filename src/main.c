#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while (1){
    printf("$ ");
    char input[50];
    fgets(input, 50, stdin);
    input[strcspn(input, "\n")] = '\0';
    if (!strncmp(input, "exit", 4)) return 0;
    printf("%s: command not found\n", input);
  }
  

  return 0;
}
