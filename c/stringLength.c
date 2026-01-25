#include <stdio.h>
#include <stdlib.h>
// come vedi le 2 scritte dell array voglion dire la stessa cosa
// perche indicano entrambe solo l indirzzo non tutta la memoria
int length(char word[]);
int length(char *word) {
  int count = 0;

  while (word[count] != '\0') {
    count++;
  }
  return count;
}

int main(int argc, char *argv[]) {

  char word[100];
  int index = 0;

  printf("inserisci una parola con meno di 100 caratteri: ");
  scanf("%s", word);
  printf("\"%s\" has a length of %d\n", word, length(word));
  return EXIT_SUCCESS;
}
