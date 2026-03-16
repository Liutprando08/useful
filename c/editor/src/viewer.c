#include "set_terminal.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
void invalidateCacheFrom(int row) {
  if (!E.row_cache)
    return;
  for (int i = row; i < E.numrows; i++) {
    free(E.row_cache[i]);
    E.row_cache[i] = NULL;
  }
  E.row_cache_valid = 0;
}
int editorGetColumn() {
  if (E.cy < 0 || E.cy > E.numrows)
    return 0;
  return E.cx - E.line_offsets[E.cy];
}
int editorGetRowContent(int row, char *buf, int bufsize) {
  if (row < 0 || row > E.numrows)
    return -1;
  int start = E.line_offsets[row];
  int end = E.line_offsets[row + 1] - 1;
  int len = end - start;
  if (len >= bufsize)
    len = bufsize - 1;
  int buf_pos = 0;
  int current_pos = 0;
  for (int i = 0; i < T.pieces_count && buf_pos < len; i++) {
    piece *p = &T.pieces[i];
    int piece_start = current_pos;
    int piece_end = current_pos + p->length;
    if (piece_end > start && piece_start < end) {
      char *src =
          (p->buffer == BUFFER_ORIGINAL) ? T.original_buffer : T.add_buffer;

      int copy_start = (piece_start < start) ? start - piece_start : 0;
      int copy_end = (piece_end > end) ? end - piece_start : p->length;

      memcpy(buf + buf_pos, src + p->start + copy_start, copy_end - copy_start);
      buf_pos += copy_end - copy_start;
    }
    current_pos = piece_end;
  }
  buf[buf_pos] = '\0';
  return buf_pos;
}
char *editorGetRenderedRow(int row) {
  if (row < 0 || row >= E.numrows)
    return NULL;
  if (!E.row_cache) {
    E.row_cache = calloc(E.numrows, sizeof(char *));
    E.row_cache_rsize = calloc(E.numrows, sizeof(int));
  }
  if (E.row_cache[row]) {
    return E.row_cache[row];
  }
  int start = E.line_offsets[row];
  int end = E.line_offsets[row + 1] - 1;
  int size = end - start;
  char *row_content = malloc(size + 1);
  editorGetRowContent(row, row_content, size + 1);
  int render_size = size;
  for (int i = 0; i < size; i++) {
    if (row_content[i] == '\t') {
      render_size += KILO_TAB_STOP - 1;
    }
  }
  char *rendered = malloc(render_size + 1);
  int idx = 0;
  for (int i = 0; i < size; i++) {
    if (row_content[i] == '\t') {
      rendered[idx++] = ' ';
      while (idx % KILO_TAB_STOP != 0)
        rendered[idx++] = ' ';
    } else {
      rendered[idx++] = row_content[i];
    }
  }
  rendered[idx] = '\0';
  free(row_content);
  E.row_cache[row] = rendered;
  E.row_cache_rsize[row] = idx;
  return rendered;
}
void initLineOffset() {
  free(E.line_offsets);
  E.line_offsets = malloc(sizeof(int) * (E.numrows + 2));
  E.line_offsets[0] = 0;
  int line = 1;
  for (size_t i = 0; i < T.original_length; i++) {
    if (T.original_buffer[i] == '\n') {
      E.line_offsets[line++] = i + 1;
    }
  }
  E.line_offsets[E.numrows + 1] = T.original_length;
  E.line_offsets_capacity = E.numrows + 2;
}
void editorScroll() {
  if (E.row_cache_rsize == NULL)
    return;
  if (E.cy < E.numrows) {
    editorGetRenderedRow(E.cy);
  }
  if (E.cy < E.rowoff) {
    E.rowoff = E.cy;
  }
  if (E.cy >= E.rowoff + E.screenRows) {
    E.rowoff = E.cy - E.screenRows + 1;
  }
  if (E.row_cache_rsize[E.cy] < E.coloff) {
    E.coloff = E.row_cache_rsize[E.cy];
  }
  if (E.row_cache_rsize[E.cy] >= E.screenCols) {
    E.coloff = E.row_cache_rsize[E.cy] - E.screenCols + 1;
  }
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

  void initLineOffset();
  fclose(fp);
  E.dirty = 0;
}
