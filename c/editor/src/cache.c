#include "editor.h"
#include <stdlib.h>

void invalidateCacheFrom(int row) {
  if (!E.row_cache)
    return;
  for (int i = row; i < E.numrows; i++) {
    free(E.row_cache[i]);
    E.row_cache[i] = NULL;
  }
  E.row_cache_valid = 0;
}

char *editorGetRenderedRow(int row) {
  if (E.line_offsets == NULL)
    return 0;
  if (row < 0 || row >= E.numrows - 1)
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
