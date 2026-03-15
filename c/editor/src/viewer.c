#include "set_terminal.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
int editorRowCxToRx(erow *row, int cx) {
  int rx = 0;
  int j;
  for (j = 0; j < cx; j++) {
    if (row->chars[j] == '\t')
      rx += (KILO_TAB_STOP - 1) - (rx % KILO_TAB_STOP);
    rx++;
  }
  return rx;
}
void editorUpdateRow(erow *row) {
  int tabs = 0;
  int j;
  for (j = 0; j < row->size; j++)
    if (row->chars[j] == '\t')
      tabs++;
  free(row->render);
  row->render = malloc(row->size + tabs * (KILO_TAB_STOP - 1) + 1);
  int idx = 0;
  for (j = 0; j < row->size; j++) {
    if (row->chars[j] == '\t') {
      row->render[idx++] = ' ';
      while (idx % KILO_TAB_STOP != 0)
        row->render[idx++] = ' ';
    } else {
      row->render[idx++] = row->chars[j];
    }
  }
  row->render[idx] = '\0';
  row->rsize = idx;
}

void editorScroll() {
  E.rx = 0;
  if (E.cy < E.numrows) {
    E.rx = editorRowCxToRx(&E.row[E.cy], E.cx);
  }
  if (E.cy < E.rowoff) {
    E.rowoff = E.cy;
  }
  if (E.cy >= E.rowoff + E.screenRows) {
    E.rowoff = E.cy - E.screenRows + 1;
  }
  if (E.rx < E.coloff) {
    E.coloff = E.rx;
  }
  if (E.rx >= E.screenCols) {
    E.coloff = E.rx - E.screenCols + 1;
  }
}
void editorRowAppendString(erow *row, char *s, size_t len) {
  row->chars = realloc(row->chars, row->size + len + 1);
  memcpy(&row->chars[row->size], s, len);
  row->size += len;
  row->chars[row->size] = '\0';
  editorUpdateRow(row);
  E.dirty++;
}
void piece_table_insert(char *c) {
  int total_length = 0;
  int len = strlen(c);
  for (int i = 0; i < T.pieces_count; i++) {
    total_length += T.pieces[i].length;
  }

  if (E.cx > total_length)
    E.cx = total_length;
  if (T.add_length + len > T.add_capacity) {
    T.add_capacity *= 2;
    T.add_buffer = realloc(T.add_buffer, T.add_capacity);
  }

  int add_position = T.add_length;
  memcpy(T.add_buffer + T.add_length, c, len);
  T.add_length += len;
  int current_pos = 0;
  int piece_index = -1;
  int offset = 0;
  for (int i = 0; i < T.pieces_count; i++) {
    int piece_end = current_pos + T.pieces[i].length;
    if (E.cx <= piece_end) {
      piece_index = i;
      offset = E.cx - current_pos;
      break;
    }
    current_pos = piece_end;
  }
  if (piece_index == -1) {
    piece_index = T.pieces_count - 1;
    offset = T.pieces[piece_index].length;
  }
  piece old_piece = T.pieces[piece_index];
  int new_pieces_needed = (offset > 0 && offset < old_piece.length) ? 2 : 1;

  if (T.pieces_count + new_pieces_needed > T.pieces_capacity) {
    T.pieces_capacity *= 2;
    T.pieces = realloc(T.pieces, sizeof(piece) * T.pieces_capacity);
  }

  for (int i = T.pieces_count; i > new_pieces_needed + piece_index; i--) {
    T.pieces[i] = T.pieces[i - new_pieces_needed];
  }
  int new_index = piece_index;

  if (offset > 0) {
    T.pieces[new_index].buffer = old_piece.buffer;
    T.pieces[new_index].start = old_piece.start;
    T.pieces[new_index].length = offset;
    new_index++;
  }

  T.pieces[new_index].buffer = BUFFER_ADD;
  T.pieces[new_index].start = add_position;
  T.pieces[new_index].length = len;
  new_index++;

  if (offset < old_piece.length) {
    T.pieces[new_index].buffer = old_piece.buffer;
    T.pieces[new_index].start = old_piece.start + offset;
    T.pieces[new_index].length = old_piece.length - offset;
  }

  T.pieces_count += new_pieces_needed;
}

void editorOpen(char *filename) {
  free(E.filename);
  E.filename = strdup(filename);
  FILE *fp = fopen(filename, "rb");
  if (!fp)
    die("fopen");
  fseek(fp, 0, SEEK_END);
  T.original_length = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  T.original_buffer = malloc(++T.original_length);
  fread(T.original_buffer, 1, T.original_length - 1, fp);
  T.original_buffer[T.original_length] = '\0';
  T.pieces[0].buffer = BUFFER_ORIGINAL;
  T.pieces[0].start = 0;
  T.pieces[0].length = T.original_length;
  T.pieces_count = 1;
  E.numrows = (T.original_length > 0) ? 1 : 0;
  for (size_t i = 0; i < T.original_length; i++) {
    if (T.original_buffer[i] == '\n')
      E.numrows++;
  }
  fclose(fp);
  E.dirty = 0;
}
