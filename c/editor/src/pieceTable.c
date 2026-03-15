#include "set_terminal.h"
#include <stdlib.h>
pieceTable T;
void initPieceTable() {
  T.original_buffer = NULL;
  T.original_length = 0;
  T.add_buffer = NULL;
  T.capacity = 0;
  T.add_length = 0;
  T.pieces->length = 0;
  T.pieces->buffer = 0;
  T.pieces->start = 0;
  T.pieces_capacity = 0;
  T.pieces_count = 0;
}
