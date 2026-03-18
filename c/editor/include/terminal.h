#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <termios.h>

void die(const char *s);
void disableRawMode();
void rawMode();
int getWindowsize(int *rows, int *cols);

#endif
