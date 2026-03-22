#include "piece_table.h"
#include "editor.h"
#include <stdlib.h>

pieceTable T;

void initPieceTable() {
  T.original_buffer = NULL;
  T.original_length = 0;
  T.add_capacity = 1024;
  T.add_buffer = malloc(T.add_capacity);
  T.capacity = 0;
  T.add_length = 0;
  T.pieces_capacity = 100;
  T.pieces = malloc(sizeof(piece) * T.pieces_capacity);
  T.pieces_count = 0;
}
