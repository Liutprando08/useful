
#include "set_terminal.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
void editorDrawRows(struct abuf *ab) {
  int y;
  for (y = 0; y < E.screenRows; y++) {
    int filerow = y + E.rowoff;
    if (filerow >= E.numrows) {
      if (E.numrows == 0 && y == E.screenRows / 3) {
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
    } else {
      int len = E.row[filerow].rsize - E.coloff;
      if (len < 0)
        len = 0;
      if (len > E.screenCols)
        len = E.screenCols;
      abAppend(ab, &E.row[filerow].render[E.coloff], len);
    }
    abAppend(ab, "\x1b[K", 3);
    abAppend(ab, "\r\n", 2);
  }
}
void editorDrawStatusBar(struct abuf *ab) {
  abAppend(ab, "\x1b[7m", 4);
  char status[80], rstatus[80];
  char modestr[8];
  int modlen;
  if (E.mode == NORMAL_MODE) {
    modlen = snprintf(modestr, sizeof(modestr), "NORMAL");
  } else {
    modlen = snprintf(modestr, sizeof(modestr), "INSERT");
  }

  int len = snprintf(status, sizeof(status), "%.20s, %s",
                     E.filename ? E.filename : "[No Name]",
                     E.dirty ? "(modified)" : "");
  int rlen =
      snprintf(rstatus, sizeof(rstatus), "%d/%d", E.cy + 1, E.numrows + 1);
  if (len > E.screenCols)
    len = E.screenCols;
  abAppend(ab, status, len);
  while (len < E.screenCols) {
    if (E.screenCols - len - modlen + 1 == rlen) {
      abAppend(ab, rstatus, rlen);
      break;
    } else if (E.screenCols / 2 - len == modlen) {
      abAppend(ab, modestr, 8);
      len++;
    } else {
      abAppend(ab, " ", 1);
      len++;
    }
  }
  abAppend(ab, "\x1b[m", 3);
}

void editorSetStatusMessage(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(E.statusmsg, sizeof(E.statusmsg), fmt, ap);
  va_end(ap);
  E.statusmsg_time = time(NULL);
}

void editorDrawMessageBar(struct abuf *ab) {
  abAppend(ab, "\x1b[K", 3);
  int msglen = strlen(E.statusmsg);
  if (msglen > E.screenCols)
    msglen = E.screenCols;
  if (msglen && time(NULL) - E.statusmsg_time < 5) {
    abAppend(ab, E.statusmsg, msglen);
  }
}
