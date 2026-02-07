#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HEAP_INC 256
#define BLK_SIZE 128
#define BUF_SIZE 256

struct header {
  uint64_t size;
  struct header *next;
};

static void die(const char *msg) {
  char buf[BUF_SIZE];
  int n = snprintf(buf, BUF_SIZE, "%s: %s\n", msg, strerror(errno));
  if (n > 0) {
    write(STDERR_FILENO, buf, (size_t)n);
  }
  exit(1);
}

static void out(char *fmt, void *val, size_t sz) {
  char buf[BUF_SIZE];
  ssize_t n =
      snprintf(buf, BUF_SIZE, fmt,
               sz == sizeof(uint64_t) ? *(uint64_t *)val : *(void **)val);
  if (n < 0) {
    die("snprintf");
  }
  if (write(STDOUT_FILENO, buf, (size_t)n) < 0) {
    die("write");
  }
}

int main(void) {
  void *base = sbrk(HEAP_INC);
  if (base == (void *)-1) {
    die("sbrk");
  }

  void *blk1 = base;
  void *blk2 = (char *)base + BLK_SIZE;

  struct header *h1 = (struct header *)blk1;
  struct header *h2 = (struct header *)blk2;

  h1->size = BLK_SIZE;
  h1->next = NULL;

  h2->size = BLK_SIZE;
  h2->next = h1;

  size_t hsz = sizeof(struct header);
  size_t dsz = BLK_SIZE - hsz;

  unsigned char *d1 = (unsigned char *)blk1 + hsz;
  unsigned char *d2 = (unsigned char *)blk2 + hsz;

  memset(d1, 0, dsz);
  memset(d2, 1, dsz);

  out("first block:       %p\n", &blk1, sizeof(blk1));
  out("second block:      %p\n", &blk2, sizeof(blk2));

  out("first block size:  %lu\n", &h1->size, sizeof(h1->size));
  out("first block next:  %p\n", &h1->next, sizeof(h1->next));

  out("second block size: %lu\n", &h2->size, sizeof(h2->size));
  out("second block next: %p\n", &h2->next, sizeof(h2->next));

  for (size_t i = 0; i < dsz; i++) {
    uint64_t v = d1[i];
    out("%lu\n", &v, sizeof(v));
  }

  for (size_t i = 0; i < dsz; i++) {
    uint64_t v = d2[i];
    out("%lu\n", &v, sizeof(v));
  }

  return 0;
}
