#include "set_terminal.h"
#include <termios.h>
#include <unistd.h>

int main() {
  rawMode();
  initEditor();
  editorOpen();
  while (1) {
    editorRefreshScreen();
    editorProcessKeypress();
  }
  return 0;
}
