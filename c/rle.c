#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *letter;
  uint8_t *num;
} compressed;
compressed *rle(char *example) {
  int c;
  int i = 0;
  int n = strlen(example);
  compressed *cp = (compressed *)malloc(sizeof(compressed));
  cp->letter = (char *)malloc(n + 1);
  cp->num = (uint8_t *)calloc(n, sizeof(uint8_t));
  while ((c = *example)) {
    cp->letter[i] = *example;

    cp->num[i]++;
    example++;
    while (cp->letter[i] == *example) {
      cp->num[i]++;
      example++;
    }

    i++;
  }
  cp->letter[i] = '\0';
  return cp;
}

int main(int argc, char *argv[]) { return EXIT_SUCCESS; }
