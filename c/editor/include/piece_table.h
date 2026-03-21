#ifndef PIECE_TABLE_H
#define PIECE_TABLE_H

#include <stddef.h>

typedef enum { BUFFER_ORIGINAL, BUFFER_ADD } BufferType;

typedef struct {
  BufferType buffer;
  int start;
  int length;
} piece;

typedef struct {
  char *original_buffer;
  size_t original_length;
  int add_capacity;
  char *add_buffer;
  int capacity;
  int add_length;
  piece *pieces;
  int pieces_count;
  int pieces_capacity;
} pieceTable;

extern pieceTable T;

void initPieceTable();
void piece_table_delete();
void piece_table_delete_range(int start, int end);
void piece_table_insert(char *c);

#endif
