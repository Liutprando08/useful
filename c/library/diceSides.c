#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int diceroll(int diceSides);

int diceroll(int diceSides) {
  // srand estrae numero casuale da secondi passati da creazione unix
  // grazie a time NULL
  srand(time(NULL));
  int n = diceSides;
  int diceroll = 1 + rand() % n;
  return diceroll;
}
int main(void) {
  int diceSides;
  printf("dimmi il numero di lato il tuo dado ha\n");
  scanf("%d", &diceSides);
  int dice = diceroll(diceSides);
  printf("\ne uscito %d", dice);
  return EXIT_SUCCESS;
}
