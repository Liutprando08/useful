#include "editor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

int getCursorPosition(int *rows, int *cols) {
  char buf[32];
  unsigned int i = 0;
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
    return -1;
  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &buf[i], 1) != 1)
      break;
    if (buf[i] == 'R')
      break;
    i++;
  }
  buf[i] = '\0';
  if (buf[0] != '\x1b' || buf[1] != '[')
    return -1;
  if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
    return -1;
  return 0;
}

char *getActualLineContent() {

  int row_size = E.line_offsets[E.cy + 1] - E.line_offsets[E.cy];
  char *content = malloc(row_size + 1);

  editorGetRowContent(E.cy, content, row_size + 1);
  return content;
}

void editorMoveCursor(char key) {
  switch (key) {
  case ARROW_LEFT: {
    if (E.cx > 0) {
      E.cx--;

    } else if (E.cx == 0) {
      if (E.cy > 0) {
        E.cy--;
        E.cx = E.row_cache_rsize[E.cy] - 1;
      }
    }
    break;
  }
  case ARROW_RIGHT: {
    if (!E.row_cache[E.cy])
      break;

    int lineRenderLen = E.row_cache_rsize[E.cy] - 1;

    if (E.cx >= lineRenderLen) {
      if (E.cy < E.numrows - 1) {
        E.cy++;
        E.cx = 0;
      } else {
        break;
      }

    } else {

      E.cx++;

      char *content = getActualLineContent();

      lineRenderLen = E.row_cache[E.cy] ? E.row_cache_rsize[E.cy] : 0;

      free(content);
    }

    break;
  }
  case ARROW_DOWN:
    if (E.cy < E.numrows - 1) {
      E.cy++;
      E.cx = 0;
    }

    break;
  case ARROW_UP:
    if (E.cy > 0) {
      E.cy--;
      E.cx = 0;
    }

    break;
  }
}
