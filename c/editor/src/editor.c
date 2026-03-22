#include "editor.h"
#include "buffer.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void editorRefreshScreen() {
  editorScroll();
  struct abuf ab = ABUF_INIT;
  abAppend(&ab, "\x1b[H", 3);
  abAppend(&ab, "\x1b[?25l", 6);
  editorDrawRows(&ab);
  editorDrawStatusBar(&ab);
  editorDrawMessageBar(&ab);
  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.rowoff) + 1,
           (E.cx - E.coloff) + 1);
  abAppend(&ab, buf, strlen(buf));
  abAppend(&ab, "\x1b[?25h", 6);
  write(STDOUT_FILENO, ab.buf, ab.len);

  abFree(&ab);
}

void initEditor() {
  initPieceTable();
  E.cx = 0;
  E.cy = 0;
  E.rowoff = 0;
  E.numrows = 1;
  E.coloff = 0;
  E.mode = NORMAL_MODE;
  E.filename = NULL;
  E.dirty = 0;
  E.statusmsg[0] = '\0';
  E.statusmsg_time = 0;
  if (getWindowsize(&E.screenRows, &E.screenCols) == -1)
    die("getWindowsize");
  E.screenRows -= 2;
  invalidateCacheFrom(E.cy);
}
