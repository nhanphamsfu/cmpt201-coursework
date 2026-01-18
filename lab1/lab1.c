#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_tokens(char *src) {
  printf("Tokens:\n");
  char *saveptr = NULL;

  for (char *tok = strtok_r(src, " ", &saveptr);
       tok != NULL;
       tok = strtok_r(NULL, " ", &saveptr)) {
    printf("  %s\n", tok);
  }
}
int main(void) {
  char *buff = NULL;
  size_t size = 0;
  while (1) {
    printf("Please enter some text: ");
    fflush(stdout);
    ssize_t n = getline(&buff, &size, stdin);
    if (n < 0) {
      perror("getline failed");
      return EXIT_FAILURE;
    }
    if (n == 1 && buff[0] == '\n') break;
    if (buff[n - 1] == '\n') buff[n - 1] = '\0';
    print_tokens(buff);
  }
  free(buff);
  return 0;
}
