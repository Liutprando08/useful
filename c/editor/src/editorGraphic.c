#include "set_terminal.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
int getWindowsize(int *rows, int *cols) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}
void editorDrawRows() {
  int y;
  for (y = 0; y < E.screenRows; y++) {
    write(STDOUT_FILENO, "~\r\n", 3);
  }
}
void editorRefreshScreen() {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  editorDrawRows();
  write(STDOUT_FILENO, "\x1b[H", 3);
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
