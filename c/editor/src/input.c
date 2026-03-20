#include "editor.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
      if (seq[1] >= '0' && seq[1] <= '9') {
        if (read(STDIN_FILENO, &seq[2], 1) != 1)
          return '\x1b';
        if (seq[2] == '~') {
          switch (seq[1]) {
          case '1':
            return HOME_KEY;
          case '3':
            return DEL_KEY;
          case '4':
            return END_KEY;
          case '5':
            return PAGE_UP;
          case '6':
            return PAGE_DOWN;
          case '7':
            return HOME_KEY;
            return HOME_KEY;
          case '8':
            return END_KEY;
          }
        }
      } else {
        switch (seq[1]) {
        case 'A':
          return ARROW_UP;
        case 'B':
          return ARROW_DOWN;
        case 'C':
          return ARROW_RIGHT;
        case 'D':
          return ARROW_LEFT;
        case 'H':
          return HOME_KEY;
        case 'F':
          return END_KEY;
        }
      }
    } else if (seq[0] == 'O') {
      switch (seq[1]) {
      case 'H':
        return HOME_KEY;
      case 'F':
        return END_KEY;
      }
    }
    return '\x1b';
  } else {
    return c;
  }
}

void editorProcessKeypress() {
  static int quit_times = KILO_QUIT_TIMES;
  char c = editorReadKey();
  if (E.mode == NORMAL_MODE) {
    switch (c) {
    case CTRL_KEY('q'):
      if (E.dirty && quit_times > 0) {
        editorSetStatusMessage("WARNING!!! File has unsaved changes. "
                               "Press Ctrl-Q %d more times to quit.",
                               quit_times);
        quit_times--;
        return;
      }
      editorCleanup();
      write(STDOUT_FILENO, "\x1b[?25h", 6);
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;
    case HOME_KEY:
      E.cx = 0;
      break;
    case END_KEY:
      if (E.cy < E.numrows)
        E.cx = E.row_cache_rsize[E.cy];
      break;
    case PAGE_UP:
    case PAGE_DOWN: {
      if (c == PAGE_UP) {
        E.cy = E.rowoff;
      } else if (c == PAGE_DOWN) {
        E.cy = E.rowoff + E.screenRows - 1;
        if (E.cy > E.numrows)
          E.cy = E.numrows - 1;
      }
      int times = E.screenRows;
      while (times--)
        editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
    } break;
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
    case CTRL_KEY('l'):
      break;
    case CTRL_KEY('s'):
      editorSave();
      break;
    default:
      if (c >= 32 && c <= 126) {
        E.cx++;
      }
    }
  } else if (INSERT_MODE == E.mode) {
    switch (c) {
    case '\r':
      editorInsertNewline();
      break;
    case '\x1b':
      E.mode = NORMAL_MODE;
      E.cx--;
      break;
    case BACKSPACE:
    case CTRL_KEY('h'):
    case DEL_KEY:
      editorDelChar();
      break;
    case CTRL_KEY('l'):
      break;
    case CTRL_KEY('s'):
      editorSave();
      break;
    default:
      editorInsertChar(c);
      break;
    }
  }
  quit_times = KILO_QUIT_TIMES;
}
