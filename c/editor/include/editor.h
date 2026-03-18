#ifndef EDITOR_H
#define EDITOR_H

#define EDITOR_VERSION "0.0.1"
#define KILO_TAB_STOP 8
#define KILO_QUIT_TIMES 3
#define CTRL_KEY(x) ((x) & 0x1f)

#include <stddef.h>
#include <time.h>
#include "buffer.h"
#include "piece_table.h"
#include "terminal.h"

struct editorConfig {
  int cx, cy;
  int rowoff;
  int screenRows;
  int screenCols;
  struct termios orig;
  int coloff;
  int numrows;
  int dirty;
  char *filename;
  char statusmsg[80];
  time_t statusmsg_time;
  int mode;
  int *line_offsets;
  int line_offsets_capacity;
  char **row_cache;
  int *row_cache_rsize;
  int row_cache_valid;
};

extern struct editorConfig E;

enum modes { NORMAL_MODE, INSERT_MODE };

enum editorKey {
  BACKSPACE = 127,
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

void initEditor();
void editorRefreshScreen();
char editorReadKey();
void editorProcessKeypress();
void editorMoveCursor(char key);
void editorScroll();
void editorDrawRows(struct abuf *ab);
void editorDrawStatusBar(struct abuf *ab);
void editorSetStatusMessage(const char *fmt, ...);
void editorDrawMessageBar(struct abuf *ab);
void editorOpen(char *filename);
void editorCleanup();
void invalidateCacheFrom(int row);
int editorGetAbsolutePosition();
int editorGetRowContent(int row, char *buf, int bufsize);
char *editorGetRenderedRow(int row);
char getCharAtPos(int pos);
void initLineOffset();
void initLineOffsetFromPieces();
char *editorRowsToString(int *buflen);
void editorInsertChar(int c);
void editorSave();
void editorDelChar();
void editorDelRow(int at);
void editorInsertNewline();
char *editorPrompt(char *prompt);

#endif
