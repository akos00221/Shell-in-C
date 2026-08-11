#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while (1){
    printf("$ ");
    char input[50];
    fgets(input, 50, stdin);
    printf("%s: command not found\n", input);
  }
  

  return 0;
}
