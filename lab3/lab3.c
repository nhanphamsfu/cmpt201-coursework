#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 5
static void add(char *h[N], int *pos, int *cnt, const char *line) {
  char *s = strdup(line);
  if (!s) {
    perror("strdup");
    exit(1);
  }
  free(h[*pos]);
  h[*pos] = s;
  *pos = (*pos + 1) % N;
  if (*cnt < N)
    (*cnt)++;
}

static void print(char *h[N], int pos, int cnt) {
  int start = (pos - cnt + N) % N;
  for (int i = 0; i < cnt; i++) {
    int idx = (start + i) % N;
    fputs(h[idx], stdout);
  }
}

int main(void) {
  char *hist[N] = {NULL};
  int pos = 0;
  int cnt = 0;
  char *line = NULL;
  size_t cap = 0;
  while (1) {
    printf("Enter input: ");
    fflush(stdout);
    if (getline(&line, &cap, stdin) < 0)
      break;
    add(hist, &pos, &cnt, line);
    if (strcmp(line, "print\n") == 0) {
      print(hist, pos, cnt);
    }
  }
  free(line);
  for (int i = 0; i < N; i++)
    free(hist[i]);
  return 0;
}
