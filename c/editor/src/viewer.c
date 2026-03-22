#include "buffer.h"
#include "editor.h"
#include "piece_table.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

char getCharAtPos(int pos) {
  int current = 0;
  for (int i = 0; i < T.pieces_count; i++) {
    piece *p = &T.pieces[i];
    if (current + p->length > pos) {
      char *src =
          (p->buffer == BUFFER_ORIGINAL) ? T.original_buffer : T.add_buffer;
      return src[p->start + (pos - current)];
    }
    current += p->length;
  }
  return '\0';
}

int editorGetRowContent(int row, char *buf, int bufsize) {
  if (row < 0 || row >= E.numrows)
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

void initLineOffsetFromPieces() {
  free(E.line_offsets);
  int total_len = 0;
  for (int i = 0; i < T.pieces_count; i++) {
    total_len += T.pieces[i].length;
  }
  E.numrows = 1;
  for (int i = 0; i < total_len; i++) {
    char ch = getCharAtPos(i);
    if (ch == '\n')
      E.numrows++;
  }
  E.line_offsets = calloc(E.numrows + 2, sizeof(int));
  E.line_offsets[0] = 0;
  int line = 1;
  int pos = 0;
  while (pos < total_len) {
    char ch = getCharAtPos(pos);
    if (ch == '\n') {
      E.line_offsets[line++] = pos + 1;
    }
    pos++;
  }
  E.line_offsets[E.numrows + 1] = total_len;
  E.line_offsets_capacity = E.numrows + 2;
}

void initLineOffset() {
  free(E.line_offsets);
  E.line_offsets = calloc(E.numrows + 2, sizeof(int));
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
  if (E.cy < E.numrows - 1) {
    editorGetRenderedRow(E.cy);
  }
  if (E.cy < E.rowoff) {
    E.rowoff = E.cy;
  }
  if (E.cy >= E.rowoff + E.screenRows) {
    E.rowoff = E.cy - E.screenRows + 1;
  }
  if (E.cy < E.numrows && E.row_cache_rsize[E.cy] < E.coloff) {
    E.coloff = E.row_cache_rsize[E.cy];
  }
  if (E.cy < E.numrows && E.row_cache_rsize[E.cy] >= E.screenCols) {
    E.coloff = E.row_cache_rsize[E.cy] - E.screenCols + 1;
  }
}
void piece_table_delete() {

  // BUG #11: Add bounds check for E.cy before array access
  if (E.cy < 0 || E.cy >= E.numrows) {
    return;
  }

  int position = E.line_offsets[E.cy] + E.cx;

  int doc_length = 0;
  for (int i = 0; i < T.pieces_count; i++) {
    doc_length += T.pieces[i].length;
  }

  if (position < 0 || position >= doc_length) {
    return;
  }

  int current_pos = 0;
  int target_piece = -1;
  int offset = 0;

  for (int i = 0; i < T.pieces_count; i++) {
    if (position >= current_pos &&
        position < current_pos + T.pieces[i].length) {
      target_piece = i;
      offset = position - current_pos;
      break;
    }
    current_pos += T.pieces[i].length;
  }

  if (target_piece == -1)
    return;

  piece p = T.pieces[target_piece];

  if (offset == 0) {

    T.pieces[target_piece].start++;
    T.pieces[target_piece].length--;
  } else if (offset == p.length - 1) {

    T.pieces[target_piece].length--;
  } else {

    T.pieces[target_piece].length = offset;

    // BUG #5: Fix array bounds - loop should start at T.pieces_count - 1, not T.pieces_count
    // and go down to target_piece + 1 (inclusive)
    for (int i = T.pieces_count; i > target_piece + 1; i--) {
      T.pieces[i] = T.pieces[i - 1];
    }

    T.pieces[target_piece + 1].buffer = p.buffer;
    T.pieces[target_piece + 1].start = p.start + offset + 1;
    T.pieces[target_piece + 1].length = p.length - offset - 1;
    
    // BUG #6: Add capacity check before incrementing pieces_count
    if (T.pieces_count + 1 > T.pieces_capacity) {
      // Capacity exceeded, cannot split piece
      T.pieces[target_piece].length = p.length;  // Restore original piece
      return;
    }
    T.pieces_count++;
  }

  if (T.pieces[target_piece].length == 0) {
    for (int i = target_piece; i < T.pieces_count - 1; i++) {
      T.pieces[i] = T.pieces[i + 1];
    }
    T.pieces_count--;
  }
}
void piece_table_insert(char *c) {
  int total_length = 0;
  int len = 1;
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
  int position = E.line_offsets[E.cy] + E.cx;
  int current_pos = 0;
  int piece_index = -1;
  int offset = 0;
  for (int i = 0; i < T.pieces_count; i++) {
    int piece_end = current_pos + T.pieces[i].length;
    if (position <= piece_end) {
      piece_index = i;
      offset = position - current_pos;
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
  for (int i = T.pieces_count - 1; i >= piece_index; i--) {
    T.pieces[i + new_pieces_needed] = T.pieces[i];
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
  T.original_buffer[T.original_length - 1] = '\0';
  T.pieces[0].buffer = BUFFER_ORIGINAL;
  T.pieces[0].start = 0;
  T.pieces[0].length = T.original_length - 1;
  T.pieces_count = 1;
  E.numrows = (T.original_length > 0) ? 1 : 0;
  for (size_t i = 0; i < T.original_length; i++) {
    if (T.original_buffer[i] == '\n')
      E.numrows = E.numrows + 1;
  }
  initLineOffset();
  fclose(fp);
  E.dirty = 0;
}

void editorCleanup() {
  if (E.row_cache) {
    for (int i = 0; i < E.numrows; i++) {
      free(E.row_cache[i]);
    }
    free(E.row_cache);
  }
  free(E.row_cache_rsize);
  free(E.line_offsets);
  free(E.filename);
  free(T.original_buffer);
  free(T.add_buffer);
  free(T.pieces);
}
