#define _DEFAULT_SOURCE
#define BUF_SIZE 64
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct header {
  uint64_t size;
  struct header *next;
};

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len =
      snprintf(buf, BUF_SIZE, format,
               data_size == sizeof(uint64_t) ? *(uint64_t *)data
                                             : (uintptr_t) * (void **)data);
  if (len < 0) {
    perror("snprintf failed");
  }
  write(STDOUT_FILENO, buf, len);
}

int main() {
  intptr_t bytes = 256;
  void *heap = sbrk(bytes);
  if (heap == (void *)-1) {
    perror("sbrk failed");
    exit(EXIT_FAILURE);
  }

  struct header *block0 = (struct header *)heap;
  struct header *block1 = (struct header *)(heap + bytes / 2);
  block0->size = block1->size = bytes / 2;
  block0->next = NULL;
  block1->next = block0;

  size_t offset = sizeof(struct header);
  memset(heap + offset, 0, block0->size - offset);
  memset(heap + block0->size + offset, 1, block1->size - offset);

  print_out("first block:       %p\n", &block0, sizeof(block0));
  print_out("second block:      %p\n", &block1, sizeof(block1));
  print_out("first block size:  %lu\n", &block0->size, sizeof(uint64_t));
  print_out("first block next:  %p\n", &block0->next, sizeof(block0->next));
  print_out("second block size: %lu\n", &block1->size, sizeof(uint64_t));
  print_out("second block next: %p\n", &block1->next, sizeof(block1->next));

  unsigned char *mem = (unsigned char *)heap;
  for (size_t i = offset; i < block0->size; i++) {
    print_out("%u\n", &mem[i], sizeof(unsigned char));
  }
  for (size_t i = block0->size + offset; i < bytes; i++) {
    print_out("%u\n", &mem[i], sizeof(unsigned char));
  }

  sbrk(-bytes);
  exit(EXIT_SUCCESS);
}
