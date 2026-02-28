#include "set_terminal.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
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
    editorReady();
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
  abAppend(&ab, "\x1b[H", 3);
  abAppend(&ab, "\x1b[?25l", 6);
  write(STDOUT_FILENO, ab.buf, ab.len);
  abFree(&ab);
}
char editorReady() {
  char c;
  if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
    die("read");
  return c;
}
void editorProcessKeypress() {
  char c = editorReady();
  switch (c) {
  case CTRL_KEY('q'):

    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    exit(0);
    break;
  }
}
void initEditor() {
  if (getWindowsize(&E.screenRows, &E.screenCols) == -1)
    die("getWindowsize");
}
