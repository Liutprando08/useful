#include <stdio.h>
#include <stdlib.h>

int binarySearch(int array[], int start, int number, int end, int tmp);

int main(int argc, char *argv[]) {
  int tmp = 0;

  int array[] = {1, 23, 42, 45, 65, 76, 87, 98, 9, 34, 3, 342};
  int sizeOfArray = sizeof(array) / sizeof(int);

  printf("%d\n", binarySearch(array, 0, 30, sizeOfArray - 1, tmp));

  return EXIT_SUCCESS;
}

int binarySearch(int array[], int start, int number, int end, int tmp) {
  if (tmp == end) {
    return array[tmp] == number ? 0 : 1;
  }
  if (number == array[tmp]) {
    return 0;
  }
  tmp++;
  return binarySearch(array, start, number, end, tmp);
}
