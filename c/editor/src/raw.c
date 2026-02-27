#include "set_terminal.h"
#include <stdio.h>
#include <stdlib.h>
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
