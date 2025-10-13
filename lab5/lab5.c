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
  while (free_list_ptr != NULL) {
    if (free_list_ptr->size >= size) {
      return free_list_ptr->id;
    }
    free_list_ptr = free_list_ptr->next;
  }
  return -1;
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  struct header *best_fit_ptr = NULL;
  while (free_list_ptr != NULL) {
    if (free_list_ptr->size >= size &&
        (best_fit_ptr == NULL || best_fit_ptr->size > free_list_ptr->size)) {
      best_fit_ptr = free_list_ptr;
      best_fit_id = best_fit_ptr->id;
    }
    free_list_ptr = free_list_ptr->next;
  }
  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  int worst_fit_id = -1;
  struct header *worst_fit_ptr = NULL;
  while (free_list_ptr != NULL) {
    if (free_list_ptr->size >= size &&
        (worst_fit_ptr == NULL || worst_fit_ptr->size < free_list_ptr->size)) {
      worst_fit_ptr = free_list_ptr;
      worst_fit_id = worst_fit_ptr->id;
    }
    free_list_ptr = free_list_ptr->next;
  }
  return worst_fit_id;
}

int main(void) {

  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

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

  for (struct header *tmp = free_list_ptr; tmp != NULL;) {
    struct header *next = tmp->next;
    free(tmp);
    tmp = next;
  }

  return 0;
}

// Part2
// Loop through free list from head:
// - Calculate left and right adjacent addresses
// - If adjacent to left: expand freed block, update next pointer, remove
// current block
// - If adjacent to right: expand current block
