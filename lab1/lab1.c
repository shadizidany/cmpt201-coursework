#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *line = NULL;
  size_t len = 0;
  printf("\nPlease enter some text: ");
  if (getline(&line, &len, stdin) == -1) {
    perror("getline failed");
    exit(EXIT_FAILURE);
  }
  char *saveptr = NULL;
  char *token = strtok_r(line, " ", &saveptr);
  printf("Tokens:\n");
  do {
    printf("  %s\n", token);
  } while ((token = strtok_r(NULL, " ", &saveptr)));
  free(line);
  exit(EXIT_SUCCESS);
}
