#include "set_terminal.h"
#include <termios.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  rawMode();
  initEditor();
  if (argc >= 2) {

    editorOpen(argv[1]);
  }

  editorSetStatusMessage("HELP: Ctrl-S = save | Ctrl-Q = quit");
  while (1) {
    editorRefreshScreen();
    editorProcessKeypress();
  }
  return 0;
}
