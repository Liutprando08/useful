#ifndef SET_TERMINAL_H
#define SET_TERMINAL_H
#include <termios.h>
#define CTRL_KEY(x) ((x) & 0x1f)
#define ABUF_INIT {NULL, 0}
#define EDITOR_VERSION "0.0.1"
struct editorConfig {
  int screenRows;
  int screenCols;
  struct termios orig;
};
struct abuf {
  char *buf;
  int len;
};
extern struct editorConfig E;
void die(const char *s);
void disableRawMode();
void rawMode();
void editorDrawRows(struct abuf *ab);
void editorRefreshScreen();
char editorReady();
void editorProcessKeypress();
int getWindowsize(int *rows, int *cols);
void initEditor();
#endif // !SET_TERMINAL_H
