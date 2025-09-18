#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char *line = NULL;
  size_t len = 0;
  while (printf("Enter programs to run.\n") &&
         getline(&line, &len, stdin) != -1) {
    char *saveptr = NULL;
    char *path = strtok_r(line, "\n", &saveptr);
    pid_t pid = fork();
    if (pid == -1) {
      perror("fork failed");
      free(line);
      exit(EXIT_FAILURE);
    } else if (!pid) {
      execl(path, "", (char *)0);
      perror("execl failed");
      exit(EXIT_FAILURE);
    } else {
      int wstatus;
      waitpid(pid, &wstatus, 0);
    }
  }
  free(line);
  exit(EXIT_SUCCESS);
}
