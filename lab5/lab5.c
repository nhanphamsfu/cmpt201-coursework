#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
struct header {
  uint64_t size;
  struct header *next;
  int id;
};
void initialize_block(struct header *block, uint64_t size, struct header *next,
                      int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}
int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  struct header *cur = free_list_ptr;
  while (cur != NULL) {
    if (cur->size >= size) {
      return cur->id;
    }
    cur = cur->next;
  }
  return -1;
}
int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  uint64_t best_fit_size = 0;
  struct header *cur = free_list_ptr;
  while (cur != NULL) {
    if (cur->size >= size) {
      if (best_fit_id == -1 || cur->size < best_fit_size) {
        best_fit_id = cur->id;
        best_fit_size = cur->size;
      }
    }
    cur = cur->next;
  }
  return best_fit_id;
}
int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  int worst_fit_id = -1;
  uint64_t worst_fit_size = 0;
  struct header *cur = free_list_ptr;
  while (cur != NULL) {
    if (cur->size >= size) {
      if (worst_fit_id == -1 || cur->size > worst_fit_size) {
        worst_fit_id = cur->id;
        worst_fit_size = cur->size;
      }
    }
    cur = cur->next;
  }

  return worst_fit_id;
}
int main(void) {
  struct header *free_block1 = (struct header*) malloc(sizeof(struct header));
  struct header *free_block2 = (struct header*) malloc(sizeof(struct header));
  struct header *free_block3 = (struct header*) malloc(sizeof(struct header));
  struct header *free_block4 = (struct header*) malloc(sizeof(struct header));
  struct header *free_block5 = (struct header*) malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  printf("The ID for First-Fit algorithm is: %d\n", first_fit_id);
  printf("The ID for Best-Fit algorithm is: %d\n", best_fit_id);
  printf("The ID for Worst-Fit algorithm is: %d\n", worst_fit_id);

  free(free_block1);
  free(free_block2);
  free(free_block3);
  free(free_block4);
  free(free_block5);
  return 0;
}
/*
Pseudo-code for coalescing contiguous free blocks
If free list is empty:
     new_block->next = NULL
     free_list = new_block
     return

Find insertion point by address:
     prev = NULL
     cur = free_list
     while cur != NULL and start(cur) < start(new_block):
         prev = cur
         cur = cur->next

Insert new_block:
     new_block->next = cur
     if prev == NULL:
         free_list = new_block
     else:
         prev->next = new_block

Merge with next if adjacent:
     if cur != NULL and end(new_block) == start(cur):
         new_block->size += sizeof(struct header) + cur->size
         new_block->next = cur->next

Merge with prev if adjacent:
     if prev != NULL and end(prev) == start(new_block):
         prev->size += sizeof(struct header) + new_block->size
         prev->next = new_block->next
*/
