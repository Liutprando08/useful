#ifndef SET_TERMINAL_H
#define SET_TERMINAL_H

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE
#define CTRL_KEY(x) ((x) & 0x1f)
#define ABUF_INIT {NULL, 0}
#define EDITOR_VERSION "0.0.1"
#define KILO_TAB_STOP 8

#include <stddef.h>
#include <termios.h>

typedef struct erow {
  int size;
  int rsize;
  char *chars;
  char *render;
} erow;
struct editorConfig {
  int cx, cy;
  int rx;
  int rowoff;
  int screenRows;
  int screenCols;
  struct termios orig;
  int coloff;
  int numrows;
  erow *row;
  int mode;
};
struct abuf {
  char *buf;
  int len;
};
enum modes { NORMAL_MODE, INSERT_MODE };
enum editorKey {
  ARROW_LEFT = -24,
  ARROW_DOWN,
  ARROW_UP,
  ARROW_RIGHT,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};
extern struct editorConfig E;
void die(const char *s);
void disableRawMode();
void rawMode();
void editorDrawRows(struct abuf *ab);
void editorRefreshScreen();
char editorReadKey();
void editorProcessKeypress();
int getWindowsize(int *rows, int *cols);
void initEditor();
void editorOpen(char *filename);
void editorAppendRow(char *s, size_t len);
void editorScroll();
void editorUpdateRow(erow *row);
#endif
