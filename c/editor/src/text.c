#include "buffer.h"
#include "cursor.h"
#include "editor.h"
#include "piece_table.h"
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>
void editorInsertChar(int c) {
  char buf[2] = {c, '\0'};

  piece_table_insert(buf);

  for (int i = E.cy + 1; i <= E.numrows; i++) {
    E.line_offsets[i]++;
    ;
  }

  E.cx++;

  invalidateCacheFrom(E.cy);
  E.dirty++;
}

char *editorRowsToString(int *buflen) {
  int totlen = 0;

  for (int i = 0; i < T.pieces_count; i++) {
    totlen += T.pieces[i].length;
  }

  *buflen = totlen;
  char *buf = malloc(totlen + 1);
  char *p = buf;

  for (int i = 0; i < T.pieces_count; i++) {
    piece *piece = &T.pieces[i];
    char *src =
        (piece->buffer == BUFFER_ORIGINAL) ? T.original_buffer : T.add_buffer;
    memcpy(p, src + piece->start, piece->length);
    p += piece->length;
  }

  *p = '\0';
  return buf;
}

void editorSave() {
  if (E.filename == NULL) {
    E.filename = editorPrompt("save as: %s(ESC to cancel)");
    if (E.filename == NULL) {
      editorSetStatusMessage("Save aborted");
      return;
    }
  }
  int len;
  char *buf = editorRowsToString(&len);
  int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
  if (fd != -1) {
    if (ftruncate(fd, len) != -1) {
      if (write(fd, buf, len) == len) {
        close(fd);
        free(buf);
        E.dirty = 0;
        editorSetStatusMessage("%d bytes written to disk", len);
        return;
      }
    }
    close(fd);
  }
  free(buf);
  editorSetStatusMessage("Can't save! I/O error: %s", strerror(errno));
}
void editorDelRow(int at) {
  if (at < 0 || at >= E.numrows)
    return;
  for (int i = at; i < E.numrows; i++) {
    E.line_offsets[i] = E.line_offsets[i + 1];
  }
  E.numrows--;

  invalidateCacheFrom(at);
  E.dirty++;
}
void editorInsertNewline() {

  invalidateCacheFrom(E.cy);
  piece_table_insert("\n");
  initLineOffsetFromPieces();
  E.cy++;
  E.cx = 0;
  E.dirty++;
}
void editorDelChar() {
  if (E.cx == 0 && E.cy == 0)
    return;

  invalidateCacheFrom(E.cy);

  if (E.cx == 0) {

    if (!E.row_cache || !E.row_cache_rsize || E.cy <= 0) {

      E.cy--;
      E.numrows--;
      for (int i = E.cy + 1; i <= E.numrows; i++) {
        E.line_offsets[i] -= 1;
      }
      E.dirty++;
      return;
    }
    E.cx = E.row_cache_rsize[E.cy - 1];
    E.cy--;

    piece_table_delete();

    if (E.row_cache) {
      free(E.row_cache[E.cy + 1]);
    }

    for (int i = E.cy + 1; i < E.numrows; i++) {

      E.line_offsets[i] = E.line_offsets[i + 1] - 1;

      if (E.row_cache) {
        E.row_cache[i] = E.row_cache[i + 1];
        E.row_cache_rsize[i] = E.row_cache_rsize[i + 1];
      }
    }

    E.numrows--;
    if (E.row_cache) {
      E.row_cache[E.numrows] = NULL;
      E.row_cache_rsize[E.numrows] = 0;
    }

    invalidateCacheFrom(E.cy);
    E.dirty++;
    return;
  }

  int rendered_width;

  rendered_width = 1;
  E.cx -= 1;

  piece_table_delete();

  for (int i = E.cy + 1; i <= E.numrows; i++) {
    E.line_offsets[i]--;
  }

  if (E.row_cache_rsize && E.row_cache_rsize[E.cy] > 0) {
    E.row_cache_rsize[E.cy] -= rendered_width;
  }

  E.dirty++;
}
char *editorPrompt(char *prompt) {
  size_t bufsize = 128;
  char *buf = malloc(bufsize);
  size_t buflen = 0;
  buf[0] = '\0';
  while (1) {
    editorSetStatusMessage(prompt, buf);
    editorRefreshScreen();
    int c = editorReadKey();
    if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
      if (buflen != 0)
        buf[--buflen] = '\0';
    } else if (c == '\x1b') {
      editorSetStatusMessage("");
      free(buf);
      return NULL;
    }
    if (c == '\r') {
      if (buflen != 0) {
        editorSetStatusMessage("");
        return buf;
      }
    } else if (!iscntrl(c) && c < 128) {
      if (buflen == bufsize - 1) {
        bufsize *= 2;
        buf = realloc(buf, bufsize);
      }
      buf[buflen++] = c;
      buf[buflen] = '\0';
    }
  }
}
