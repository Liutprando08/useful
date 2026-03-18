#include "buffer.h"
#include "editor.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

struct editorConfig E;

void die(const char *s) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  perror(s);
  exit(1);
}

void disableRawMode() {
  write(STDOUT_FILENO, "\x1b[?25h", 6);
  write(STDOUT_FILENO, "\x1b[0m", 4);
  write(STDOUT_FILENO, "\x0f", 1);
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  write(STDOUT_FILENO, "\x1b[?1l", 5);
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig) == -1)
    die("tcsetattr");
}

void rawMode() {
  if (tcgetattr(STDIN_FILENO, &E.orig) == -1)
    die("tcgetattr");

  atexit(disableRawMode);

  struct termios raw;
  raw = E.orig;
  raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tcsetattr");
}

int getWindowsize(int *rows, int *cols) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
      return -1;
    editorReadKey();
    return -1;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}
