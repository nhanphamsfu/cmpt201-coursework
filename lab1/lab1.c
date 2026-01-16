#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void print_tokens(char *src, ssize_t nread) {
  printf("Tokens:\n");
  char *saveptr = NULL;
  for (char *tok = strtok_r(src, " ", &saveptr); tok != NULL;
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
    ssize_t num_char = getline(&buff, &size, stdin);
    if (num_char < 0) {
      perror("getline failed");
      return EXIT_FAILURE;
    }
    if (num_char == 1 && buff[0] == '\n') {
      break;
    }
    if (num_char > 0 && buff[num_char - 1] == '\n') {
      buff[num_char - 1] = '\0';
      num_char -= 1;
    }
    print_tokens(buff, num_char);
  }
  free(buff);
  return 0;
}
