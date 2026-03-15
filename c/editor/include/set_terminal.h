#ifndef SET_TERMINAL_H
#define SET_TERMINAL_H

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE
#define CTRL_KEY(x) ((x) & 0x1f)
#define ABUF_INIT {NULL, 0}
#define EDITOR_VERSION "0.0.1"
#define KILO_TAB_STOP 8
#define KILO_QUIT_TIMES 3

#include <stddef.h>
#include <termios.h>
#include <time.h>

struct editorConfig {
  int cx, cy;
  int rx;
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
struct abuf {
  char *buf;
  int len;
};

enum modes { NORMAL_MODE, INSERT_MODE };
typedef enum { BUFFER_ORIGINAL, BUFFER_ADD } BufferType;
typedef struct {
  BufferType buffer;
  int start;
  int length;
} piece;
typedef struct {
  char *original_buffer;
  size_t original_length;
  int add_capacity;
  char *add_buffer;
  int capacity;
  int add_length;
  piece *pieces;
  int pieces_count;
  int pieces_capacity;

} pieceTable;
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
extern struct editorConfig E;

extern pieceTable T;
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
void editorScroll();
void piece_table_insert(char *c);
void editorUpdateRow(erow *row);
void abAppend(struct abuf *ab, const char *s, int len);
void abFree(struct abuf *ab);
void editorDrawStatusBar(struct abuf *ab);
void editorSetStatusMessage(const char *fmt, ...);
void editorDrawMessageBar(struct abuf *ab);
void editorRowInsertChar(erow *row, int at, int c);
char *editorRowsToString(int *buflen);
void editorInsertChar(int c);
void editorSave();
void editorRowDelChar(erow *row, int at);
void editorDelChar();
void editorFreeRow(erow *row);
void editorDelRow(int at);
void editorRowAppendString(erow *row, char *s, size_t len);
void editorInsertNewline();
char *editorPrompt(char *prompt);
void initPieceTable();
#endif
