#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/* based on https://www.lemoda.net/c/die/ */
#define die(args)                                           \
  do {                                                      \
    fprintf(stderr, "error (%s:%d): ", __FILE__, __LINE__); \
    fprintf args;                                           \
    fprintf(stderr, "\n");                                  \
    exit(EXIT_FAILURE);                                     \
  } while (0)


void *alloc(unsigned size)
{
  void *out = malloc(size);
  if (!out) die(("malloc() failed!\n"));

  return out;
}


