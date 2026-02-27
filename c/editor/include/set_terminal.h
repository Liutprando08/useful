#ifndef SET_TERMINAL_H
#define SET_TERMINAL_H
#include <termios.h>
#define CTRL_KEY(x) ((x) & 0x1f)
struct editorConfig {
  int screenRows;
  int screenCols;
  struct termios orig;
};
extern struct editorConfig E;
void die(const char *s);
void disableRawMode();
void rawMode();
void editorDrawRows();
void editorRefreshScreen();
char editorReady();
void editorProcessKeypress();
int getWindowsize(int *rows, int *cols);
void initEditor();
#endif // !SET_TERMINAL_H
