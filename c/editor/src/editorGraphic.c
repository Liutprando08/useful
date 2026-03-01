#include "set_terminal.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
void editorMoveCursor(char key) {
  switch (key) {
  case ARROW_LEFT:
    if (E.cx > 0) {
      E.cx--;
    }
    break;
  case ARROW_RIGHT:
    if (E.cx != E.screenCols - 1) {
      E.cx++;
    }
    break;

  case ARROW_DOWN:
    if (E.cy != E.screenRows - 1) {

      E.cy++;
    }
    break;
  case ARROW_UP:
    if (E.cy > 0) {

      E.cy--;
    }
    break;
  }
}
void abAppend(struct abuf *ab, const char *s, int len) {
  char *new = realloc(ab->buf, ab->len + len);
  if (new == NULL)
    return;
  memcpy(&new[ab->len], s, len);
  ab->buf = new;
  ab->len += len;
}
void abFree(struct abuf *ab) { free(ab->buf); }
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
int getWindowsize(int *rows, int *cols) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
      return -1;
    editorReadKey();
    return -1;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}
void editorDrawRows(struct abuf *ab) {
  int y;
  for (y = 0; y < E.screenRows; y++) {
    if (y == E.screenRows / 3) {
      char welcome[80];
      int welcomelen = snprintf(welcome, sizeof(welcome),
                                "Kilo editor -- version %s", EDITOR_VERSION);
      if (welcomelen > E.screenCols)
        welcomelen = E.screenCols;
      int padding = (E.screenCols - welcomelen) / 2;
      if (padding) {
        abAppend(ab, "~", 1);
        padding--;
      }
      while (padding--)
        abAppend(ab, " ", 1);
      abAppend(ab, welcome, welcomelen);
    } else {
      abAppend(ab, "~", 1);
    }
    abAppend(ab, "\x1b[K", 3);
    if (y < E.screenRows - 1) {
      abAppend(ab, "\r\n", 2);
    }
  }
}
void editorRefreshScreen() {
  struct abuf ab = ABUF_INIT;
  abAppend(&ab, "\x1b[H", 3);
  abAppend(&ab, "\x1b[?25l", 6);
  editorDrawRows(&ab);
  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
  abAppend(&ab, buf, strlen(buf));
  abAppend(&ab, "\x1b[?25h", 6);
  write(STDOUT_FILENO, ab.buf, ab.len);
  abFree(&ab);
}
char editorReadKey() {
  char c;
  int nread;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN)
      die("read");
  }
  if ('\x1b' == c) {
    char seq[3];
    if (read(STDIN_FILENO, &seq[0], 1) != 1)
      return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1)
      return '\x1b';
    if (seq[0] == '[') {
      switch (seq[1]) {
      case 'A':
        return ARROW_UP;
      case 'B':
        return ARROW_DOWN;
      case 'C':
        return ARROW_RIGHT;
      case 'D':
        return ARROW_LEFT;
      }
    }
    return '\x1b';
  }
  return c;
}
void editorProcessKeypress() {
  char c = editorReadKey();
  if (E.mode == NORMAL_MODE) {

    switch (c) {
    case CTRL_KEY('q'):
      write(STDOUT_FILENO, "\x1b[?25h", 6);
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;
    case ARROW_LEFT:
    case ARROW_DOWN:
    case ARROW_UP:
    case ARROW_RIGHT:
      editorMoveCursor(c);
      break;
    case 'h':
      editorMoveCursor(ARROW_LEFT);
      break;
    case 'j':
      editorMoveCursor(ARROW_DOWN);
      break;
    case 'k':
      editorMoveCursor(ARROW_UP);
      break;
    case 'l':
      editorMoveCursor(ARROW_RIGHT);
      break;
    case 'i':
      E.mode = INSERT_MODE;
      break;
    case 'a':
      E.cx++;
      E.mode = INSERT_MODE;
      break;
    default:
      if (c >= 32 && c <= 126) {
        E.cx++;
      }
    }
  } else if (INSERT_MODE == E.mode) {
    switch (c) {
    case '\x1b':
      E.mode = NORMAL_MODE;
      break;
    default:
      if (c >= 32 && c <= 126) {
        E.cx++;
      }
    }
  }
}
void initEditor() {
  E.cx = 0;
  E.cy = 0;
  E.mode = NORMAL_MODE;
  if (getWindowsize(&E.screenRows, &E.screenCols) == -1)
    die("getWindowsize");
}
