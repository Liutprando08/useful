#include <stdio.h>
#include <stdlib.h>
/*
 *Se lanci il comando: ./test -v -f file.txt

argc sarà 4.

argv[0] -> ./test

argv[1] -> -v

argv[2] -> -f

argv[3] -> file.txt
environ e un array di stringhe che contiene variabili di ambiente
unix
  */

int main(int argc, char **argv, char **environ) {
  int i = -1;
  while (environ[++i]) {
    printf("%s\n", environ[i]);
  }
  return EXIT_SUCCESS;
}
