#ifndef SET_TERMINAL_H
#define SET_TERMINAL_H
#include <termios.h>
#define CTRL_KEY(x) ((x) & 0x1f)
#define ABUF_INIT {NULL, 0}
#define EDITOR_VERSION "0.0.1"
typedef struct erow {
  int size;
  char *chars;

} erow;
struct editorConfig {
  int cx, cy;
  int screenRows;
  int screenCols;
  struct termios orig;
  int numrows;
  erow row;
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
void editorOpen();

#endif // !SET_TERMINAL_H
