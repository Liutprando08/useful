#include "editor.h"
#include <errno.h>
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

void editorMoveCursor(char key) {
  switch (key) {
  case ARROW_LEFT:
    if (E.cx > 0) {
      E.cx--;
    } else if (E.cx == 0) {
      if (E.cy > 0) {
        E.cy--;
        E.cx = E.row_cache_rsize[E.cy];
      }
    }
    break;
  case ARROW_RIGHT:
    if (E.row_cache[E.cy] && E.cx < E.row_cache_rsize[E.cy]) {
      E.cx++;
    } else if (E.row_cache[E.cy] && E.cx == E.row_cache_rsize[E.cy]) {
      if (E.cy < E.numrows - 1) { // ADD THIS CHECK
        E.cy++;
        E.cx = 0;
      }
    }
    break;
  case ARROW_DOWN:
    if (E.cy < E.numrows - 1) {
      E.cy++;
    }
    break;
  case ARROW_UP:
    if (E.cy > 0) {
      E.cy--;
    }
    break;
  }
  int rowlen = E.row_cache[E.cy] ? E.row_cache_rsize[E.cy] : 0;
  if (E.cx > rowlen) {
    E.cx = rowlen;
  }
}
