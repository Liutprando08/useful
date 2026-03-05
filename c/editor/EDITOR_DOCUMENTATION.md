# Terminal Text Editor - Complete Code Documentation

<!--toc:start-->
- [Terminal Text Editor - Complete Code Documentation](#terminal-text-editor-complete-code-documentation)
  - [Table of Contents](#table-of-contents)
  - [set_terminal.h](#setterminalh)
  - [main.c](#mainc)
  - [raw.c](#rawc)
  - [viewer.c](#viewerc)
  - [editorGraphic.c](#editorgraphicc)
  - [Summary](#summary)
<!--toc:end-->

## Table of Contents

1. [set_terminal.h](#set_terminalh) - Header file
2. [main.c](#mainc) - Entry point
3. [viewer.c](#viewerc) - File loading
4. [raw.c](#rawc) - Terminal mode management
5. [editorGraphic.c](#editorgraphicc) - Core editor functionality

---

## set_terminal.h

```c
#ifndef SET_TERMINAL_H
#define SET_TERMINAL_H
```

Line 1: Header guard start - prevents the header from being included multiple times in the same compilation unit.

Line 2: Defines the macro `SET_TERMINAL_H` to indicate the header has been included.

```c
#define _DEFAULT_SOURCE
```

Line 3: Feature test macro that enables definitions from various POSIX and BSD sources. Required for some functions like `getline()`.

```c
#define _BSD_SOURCE
```

Line 4: Feature test macro that exposes BSD-specific definitions.

```c
#define _GNU_SOURCE
```

Line 5: Feature test macro that exposes GNU-specific extensions.

```c
#define CTRL_KEY(x) ((x) & 0x1f)
```

Line 6: Macro to convert a character to its control key equivalent. For example, CTRL_KEY('c') = 0x03. The bitwise AND with 0x1f (binary 00011111) strips the upper 3 bits, giving the control character.

```c
#define ABUF_INIT {NULL, 0}
```

Line 7: Macro for initializing an append buffer (abuf struct) with NULL buffer and 0 length.

```c
#define EDITOR_VERSION "0.0.1"
```

Line 8: Defines the editor's version string displayed in the welcome message.

```c
#include <stddef.h>
```

Line 9: Includes standard definitions, provides `size_t` and `NULL` definitions.

```c
#include <termios.h>
```

Line 10: Includes the termios header for terminal control functions (tcgetattr, tcsetattr, etc.).

```c
typedef struct erow {
  int size;
  char *chars;
} erow;
```

Lines 11-14: Defines the `erow` (editor row) structure:

- `size`: Integer storing the length of the row in characters
- `chars`: Pointer to dynamically allocated character array containing the row text

```c
struct editorConfig {
  int cx, cy;
  int rowoff;
  int screenRows;
  int screenCols;
  struct termios orig;

  int numrows;
  erow *row;
  int mode;
};
```

Lines 15-26: Defines the main editor configuration structure:

- `cx`: Cursor X position (column), 0-indexed
- `cy`: Cursor Y position (row), 0-indexed
- `rowoff`: Vertical scroll offset (row number shown at top of screen)
- `screenRows`: Number of rows in the terminal screen
- `screenCols`: Number of columns in the terminal screen
- `orig`: Stores original terminal attributes for restoration
- `numrows`: Number of rows (lines) currently in the buffer
- `row`: Pointer to array of editor rows (dynamically allocated)
- `mode`: Current editor mode (NORMAL_MODE or INSERT_MODE)

```c
struct abuf {
  char *buf;
  int len;
};
```

Lines 27-30: Defines an append buffer structure for efficient string building:

- `buf`: Pointer to character buffer
- `len`: Current length of data in buffer

```c
enum modes { NORMAL_MODE, INSERT_MODE };
```

Line 31: Enumeration defining two editor modes:

- `NORMAL_MODE`: Default mode for navigation and commands (vim-like)
- `INSERT_MODE`: Mode for inserting/text input

```c
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
```

Lines 32-42: Enumeration for special editor keys with negative values to avoid conflict with regular ASCII characters:

- Arrow keys: LEFT, DOWN, UP, RIGHT (starting at -24)
- DEL_KEY: Delete key
- HOME_KEY: Home key
- END_KEY: End key
- PAGE_UP/PAGE_DOWN: Page navigation keys

```c
extern struct editorConfig E;
```

Line 43: Declares the global editor configuration variable E (defined in raw.c). The `extern` keyword indicates it's defined elsewhere.

```c
void die(const char *s);
```

Line 44: Function declaration for error handling function that prints error message and exits.

```c
void disableRawMode();
```

Line 45: Function declaration to restore terminal to original state.

```c
void rawMode();
```

Line 46: Function declaration to enable raw terminal mode.

```c
void editorDrawRows(struct abuf *ab);
```

Line 47: Function declaration to draw all rows to the append buffer.

```c
void editorRefreshScreen();
```

Line 48: Function declaration to refresh the screen (clear and redraw).

```c
char editorReadKey();
```

Line 49: Function declaration to read a keypress, handling escape sequences.

```c
void editorProcessKeypress();
```

Line 50: Function declaration to process keypresses based on current mode.

```c
int getWindowsize(int *rows, int *cols);
```

Line 51: Function declaration to get terminal window size.

```c
void initEditor();
```

Line 52: Function declaration to initialize the editor.

```c
void editorOpen(char *filename);
```

Line 53: Function declaration to open and load a file.

```c
void editorAppendRow(char *s, size_t len);
```

Line 54: Function declaration to append a row to the editor's buffer.

```c
void editorScroll();
```

Line 55: Function declaration to calculate scroll offset based on cursor position.

```c
#endif
```

Line 56: Ends the header guard conditional compilation.

---

## main.c

```c
#include "set_terminal.h"
```

Line 1: Includes the custom header file containing all editor definitions and function declarations.

```c
#include <termios.h>
```

Line 2: Includes the termios header for terminal control functions.

```c
#include <unistd.h>
```

Line 3: Includes POSIX API header for read, write, and other system calls.

```c
int main(int argc, char *argv[]) {
```

Line 4: Main function with command-line arguments:

- `argc`: Argument count (number of command-line arguments)
- `argv`: Argument vector (array of character pointers, the actual arguments)

```c
  rawMode();
```

Line 5: Enables raw terminal mode - disables echo, canonical mode, and other terminal features to allow immediate character input.

```c
  initEditor();
```

Line 6: Initializes the editor configuration structure E with default values (cursor position, screen size, mode, row pointer).

```c
  if (argc >= 2) {
```

Line 7: Checks if at least 2 arguments were provided (program name + filename).

```c
    editorOpen(argv[1]);
```

Line 8: If a filename was provided, call editorOpen to load it. `argv[1]` is the first command-line argument (the filename).

```c
  }
```

Line 9: Closes the if statement.

```c
  while (1) {
```

Line 10: Infinite loop - the main editor loop that runs until exit.

```c
    editorRefreshScreen();
```

Line 11: Clears the screen and redraws the editor content at current cursor position.

```c
    editorProcessKeypress();
```

Line 12: Reads a keypress and processes it according to the current mode (normal/insert), performing the appropriate action.

```c
  }
```

Line 13: Closes the infinite while loop.

```c
  return 0;
```

Line 14: This line is technically unreachable due to the infinite loop, but satisfies the int main return type requirement.

```c
}
```

Line 15: Closes the main function.

---

## raw.c

```c
#include "set_terminal.h"
```

Line 1: Includes the custom header file.

```c
#include <stdio.h>
```

Line 2: Standard I/O for perror function.

```c
#include <stdlib.h>
```

Line 3: Standard library for exit function.

```c
#include <termios.h>
```

Line 4: Terminal control functions and structures.

```c
#include <unistd.h>
```

Line 5: POSIX API for read, write.

```c
struct editorConfig E;
```

Line 6: Defines the global editor configuration variable E. This is the single instance of the editor state.

```c
void die(const char *s) {
```

Line 7: Error handling function that prints an error message and exits. Takes a string describing what failed.

```c
  write(STDOUT_FILENO, "\x1b[2J", 4);
```

Line 8: Clears the entire screen using ANSI escape sequence:

- `\x1b` = ESC character (ASCII 27)
- `[2J` = Clear entire screen

```c
  write(STDOUT_FILENO, "\x1b[H", 3);
```

Line 9: Moves cursor to home position (top-left) using ANSI escape:

- `[H` = Cursor home

```c
  perror(s);
```

Line 10: Prints the error message. Uses errno to print "s: error message" to stderr.

```c
  exit(1);
```

Line 11: Exits the program with exit code 1 (indicating error).

```c
}
```

Line 12: Closes the die function.

```c
void disableRawMode() {
```

Line 13: Function to restore terminal to its original state. Called on normal exit or error.

```c
  write(STDOUT_FILENO, "\x1b[?25h", 6);
```

Line 14: Shows the cursor (ANSI escape `\x1b[?25h` = enable cursor).

```c
  write(STDOUT_FILENO, "\x1b[0m", 4);
```

Line 15: Resets all text attributes to default (turns off any formatting).

```c
  write(STDOUT_FILENO, "\x0f", 1);
```

Line 16: Normal character set (SI - Shift In).

```c
  write(STDOUT_FILENO, "\x1b[2J", 4);
```

Line 17: Clears the screen again.

```c
  write(STDOUT_FILENO, "\x1b[H", 3);
```

Line 18: Moves cursor to home position.

```c
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig) == -1)
    die("tcsetattr");
```

Lines 19-20: Restores original terminal attributes:

- `STDIN_FILENO`: File descriptor for standard input
- `TCSAFLUSH`: Waits for output to complete before applying changes
- `&E.orig`: Pointer to saved original terminal settings
- If it fails, calls die with "tcsetattr"

```c
void rawMode() {
```

Line 21: Function to configure terminal for raw mode (character-by-character input).

```c
  if (tcgetattr(STDIN_FILENO, &E.orig) == -1)
    die("tcgetattr");
```

Lines 22-23: Gets current terminal attributes and saves them to E.orig:

- `tcgetattr`: Gets terminal settings
- If it fails, calls die with "tcgetattr"

```c
  atexit(disableRawMode);
```

Line 24: Registers disableRawMode to be called automatically when the program exits normally. This ensures terminal is always restored.

```c
  struct termios raw;
  raw = E.orig;
```

Lines 25-26: Creates a copy of original terminal settings to modify.

```c
  raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
```

Line 27: Disables input processing flags:

- `IXON`: Disable software flow control (Ctrl-S/Q)
- `ICRNL`: Disable carriage return to newline conversion
- `BRKINT`: Disable break signal
- `INPCK`: Disable parity checking
- `ISTRIP`: Disable stripping 8th bit

```c
  raw.c_oflag &= ~(OPOST);
```

Line 28: Disables output processing (OPOST - output post-processing).

```c
  raw.c_cflag |= (CS8);
```

Line 29: Sets character size to 8 bits (CS8).

```c
  raw.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);
```

Line 30: Disables local flags:

- `ICANON`: Disable canonical mode (line buffering) - read returns immediately
- `ECHO`: Disable echo - don't print typed characters
- `ISIG`: Disable signal generation (Ctrl-C/Z)
- `IEXTEN`: Disable extended input processing

```c
  raw.c_cc[VMIN] = 0;
```

Line 31: Sets VMIN (minimum characters for non-blocking read) to 0.

```c
  raw.c_cc[VTIME] = 1;
```

Line 32: Sets VTIME (timeout in tenths of a second) to 1 (0.1 seconds).

```c
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tcsetattr");
```

Lines 33-34: Applies the raw terminal settings. If it fails, calls die.

```c
}
```

Line 35: Closes the rawMode function.

---

## viewer.c

```c
#include "set_terminal.h"
```

Line 1: Includes the custom header file.

```c
#include <stddef.h>
```

Line 2: Includes for size_t and NULL definitions.

```c
#include <stdio.h>
```

Line 3: Standard I/O for FILE, fopen, fclose, getline.

```c
#include <stdlib.h>
```

Line 4: Standard library for malloc, realloc, free.

```c
#include <string.h>
```

Line 5: String functions for memcpy.

```c
#include <sys/types.h>
```

Line 6: System types (ssize_t).

```c
#include <unistd.h>
```

Line 7: POSIX API (for file operations).

```c
void editorScroll() {
```

Line 8: Function to calculate the vertical scroll offset based on cursor position.

```c
  if (E.cy < E.rowoff) {
    E.rowoff = E.cy;
  }
```

Lines 9-11: If cursor is above the visible screen area, scroll up to show cursor.

```c
  if (E.cy >= E.rowoff + E.screenRows) {
    E.rowoff = E.cy - E.screenRows + 1;
  }
```

Lines 12-14: If cursor is below the visible screen area, scroll down to show cursor.

```c
}
```

Line 15: Closes the editorScroll function.

```c
void editorAppendRow(char *s, size_t len) {
```

Line 16: Function to append a new row to the editor's row array.

```c
  E.row = realloc(E.row, sizeof(erow) * (E.numrows + 1));
```

Line 17: Reallocates the row array to accommodate one more row.

```c
  int at = E.numrows;
```

Line 18: Stores the index where the new row will be added.

```c
  E.row[at].size = len;
```

Line 19: Sets the size of the new row.

```c
  E.row[at].chars = malloc(len + 1);
```

Line 20: Allocates memory for the row's character array. Adds 1 for null terminator.

```c
  memcpy(E.row[at].chars, s, len);
```

Line 21: Copies the string content into the new row.

```c
  E.row[at].chars[len] = '\0';
```

Line 22: Null-terminates the string in the new row.

```c
  E.numrows++;
```

Line 23: Increments the number of rows.

```c
}
```

Line 24: Closes the editorAppendRow function.

```c
void editorOpen(char *filename) {
```

Line 25: Function definition - opens and loads a file into the editor. Takes a filename string parameter.

```c
  FILE *fp = fopen(filename, "r");
```

Line 26: Opens the file in read mode ("r"). Returns a FILE pointer or NULL on failure.

```c
  if (!fp)
    die("fopen");
```

Lines 27-28: If fopen returns NULL (file not found or no permission), call die with "fopen" error message and exit.

```c
  char *line = NULL;
```

Line 29: Declares a char pointer for getline to store the allocated line. Initially NULL.

```c
  size_t linecap = 0;
```

Line 30: Declares capacity variable for getline's dynamic allocation. Initial capacity 0.

```c
  ssize_t linelen;
```

Line 31: Declares variable to store the length of the line returned by getline.

```c
  while ((linelen = getline(&line, &linecap, fp)) != -1) {
```

Line 32: Loop that reads each line from the file until EOF (-1):

- `&line`: Pointer to buffer pointer (getline allocates if needed)
- `&linecap`: Pointer to capacity (getline resizes if needed)
- `fp`: File pointer
- Returns number of characters read, or -1 on error/EOF

```c
    while (linelen > 0 &&
           (line[linelen - 1] == '\n' || line[linelen - 1] == '\r'))
      linelen--;
```

Lines 33-35: Loop to strip trailing newline (`\n`) and carriage return (`\r`) characters from the end of the line.

```c
    editorAppendRow(line, linelen);
```

Line 36: Calls editorAppendRow to add the cleaned line to the editor's buffer.

```c
  }
```

Line 37: Closes the while loop.

```c
  free(line);
```

Line 38: Frees the memory allocated by getline for the line buffer.

```c
  fclose(fp);
```

Line 39: Closes the file.

```c
}
```

Line 40: Closes the editorOpen function.

---

## editorGraphic.c

```c
#include "set_terminal.h"
```

Line 1: Includes the custom header file.

```c
#include <errno.h>
```

Line 2: Error number definitions for error handling.

```c
#include <stdio.h>
```

Line 3: Standard I/O (snprintf).

```c
#include <stdlib.h>
```

Line 4: Standard library (realloc, free).

```c
#include <string.h>
```

Line 5: String functions (memcpy, strlen).

```c
#include <sys/ioctl.h>
```

Line 6: I/O control operations for getting terminal size.

```c
#include <sys/types.h>
```

Line 7: System types.

```c
#include <unistd.h>
```

Line 8: POSIX API (read, write, ioctl).

```c
void editorMoveCursor(char key) {
```

Line 9: Function to move the cursor based on key press. Takes a character representing the direction.

```c
  switch (key) {
```

Line 10: Switch statement to handle different key inputs.

```c
  case ARROW_LEFT:
```

Line 11: Case for left arrow key.

```c
    if (E.cx > 0) {
      E.cx--;
    }
```

Lines 12-14: If not at left edge (cx > 0), decrement cursor X position.

```c
    break;
```

Line 15: Break out of switch.

```c
  case ARROW_RIGHT:
```

Line 16: Case for right arrow key.

```c
    if (E.cx < E.screenCols - 1) {
      E.cx++;
    }
```

Lines 17-19: If not at right edge, increment cursor X position.

```c
    break;
```

Line 20: Break out of switch.

```c
  case ARROW_DOWN:
```

Line 21: Case for down arrow key.

```c
    if (E.cy < E.screenRows - 1) {
      E.cy++;
    }
```

Lines 22-24: If not at bottom edge, increment cursor Y position.

```c
    break;
```

Line 25: Break out of switch.

```c
  case ARROW_UP:
```

Line 26: Case for up arrow key.

```c
    if (E.cy > 0) {
      E.cy--;
    }
```

Lines 27-29: If not at top edge, decrement cursor Y position.

```c
    break;
```

Line 30: Break out of switch.

```c
  }
```

Line 31: Closes the switch statement.

```c
}
```

Line 32: Closes the editorMoveCursor function.

```c
void abAppend(struct abuf *ab, const char *s, int len) {
```

Line 33: Appends a string to the append buffer. Takes pointer to abuf, string to append, and length.

```c
  char *new = realloc(ab->buf, ab->len + len);
```

Line 34: Reallocates the buffer to accommodate new data. Calculates new size = current length + string length.

```c
  if (new == NULL)
    return;
```

Lines 35-36: If realloc fails (returns NULL), return early without appending.

```c
  memcpy(&new[ab->len], s, len);
```

Line 37: Copies the string data into the buffer at the position after existing content.

```c
  ab->buf = new;
```

Line 38: Updates the buffer pointer to the newly allocated memory.

```c
  ab->len += len;
```

Line 39: Updates the buffer length.

```c
}
```

Line 40: Closes the abAppend function.

```c
void abFree(struct abuf *ab) { free(ab->buf); }
```

Line 41: Frees the memory allocated for the append buffer. Simple one-liner.

```c
int getCursorPosition(int *rows, int *cols) {
```

Line 42: Gets the current cursor position by querying the terminal. Uses ANSI escape sequence.

```c
  char buf[32];
  unsigned int i = 0;
```

Lines 43-44: Buffer to store response, and index variable.

```c
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
    return -1;
```

Lines 45-46: Sends Device Status Report (DSR) request:

- `\x1b[6n` = ESC [ 6 n = "What is cursor position?"
- Returns -1 if write fails.

```c
  while (i < sizeof(buf) - 1) {
```

Line 47: Loop to read response (leaving room for null terminator).

```c
    if (read(STDIN_FILENO, &buf[i], 1) != 1)
      break;
```

Line 48: Reads one character. If read fails or returns 0, break loop.

```c
    if (buf[i] == 'R')
      break;
```

Lines 49-50: If we see 'R', that's the end of the response (format: `\x1b[ROW;COLR`).

```c
    i++;
```

Line 51: Increment index.

```c
  }
```

Line 52: Closes the while loop.

```c
  buf[i] = '\0';
```

Line 53: Null-terminate the response string.

```c
  if (buf[0] != '\x1b' || buf[1] != '[')
    return -1;
```

Lines 54-55: Validate response starts with ESC [.

```c
  if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
    return -1;
```

Lines 56-57: Parse the row and column values from the response. Skip `\x1b[`, parse "ROW;COL" format.

```c
  return 0;
```

Line 58: Return success.

```c
}
```

Line 59: Closes getCursorPosition function.

```c
int getWindowsize(int *rows, int *cols) {
```

Line 60: Gets terminal window size using ioctl TIOCGWINSZ.

```c
  struct winsize ws;
```

Line 61: Winsize structure to hold rows/columns.

```c
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
```

Line 62: Try ioctl first:

- `TIOCGWINSZ`: Get window size ioctl command
- If it fails or returns 0 columns, need fallback method

```c
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
      return -1;
```

Lines 63-64: Fallback: move cursor far right (999C) and far down (999B) to force terminal to report size.

```c
    editorReadKey();
```

Line 65: Read the response (the cursor position report).

```c
    return -1;
```

Line 66: Return -1 to indicate failure (getCursorPosition will be called to get actual values).

```c
  } else {
```

Line 67: Else branch when ioctl works.

```c
    *cols = ws.ws_col;
    *rows = ws.ws_row;
```

Lines 68-69: Copy the window size values to output parameters.

```c
    return 0;
```

Line 70: Return success.

```c
  }
}
```

Line 71: Closes getWindowsize function.

```c
void editorDrawRows(struct abuf *ab) {
```

Line 72: Function to draw all rows to the append buffer.

```c
  int y;
  for (y = 0; y < E.screenRows; y++) {
```

Lines 73-74: Loop through each row of the terminal screen.

```c
    int filerow = y + E.rowoff;
```

Line 75: Calculate which file row corresponds to this screen row (add scroll offset).

```c
    if (filerow >= E.numrows) {
```

Line 76: If this screen row is beyond the number of loaded rows, draw empty space or welcome message.

```c
      if (E.numrows == 0 && y == E.screenRows / 3) {
```

Line 77: Show welcome message only when file is empty and this is the middle row.

```c
        char welcome[80];
        int welcomelen = snprintf(welcome, sizeof(welcome),
                                  "Kilo editor -- version %s", EDITOR_VERSION);
```

Lines 78-80: Create welcome message string with version.

```c
        if (welcomelen > E.screenCols)
          welcomelen = E.screenCols;
```

Lines 81-82: Truncate welcome message if it's wider than the screen.

```c
        int padding = (E.screenCols - welcomelen) / 2;
```

Line 83: Calculate centering padding.

```c
        if (padding) {
          abAppend(ab, "~", 1);
          padding--;
        }
```

Lines 84-86: Add tilde with one less padding if padding exists.

```c
        while (padding--)
          abAppend(ab, " ", 1);
```

Lines 87-88: Add remaining padding spaces.

```c
        abAppend(ab, welcome, welcomelen);
```

Line 89: Add the welcome message.

```c
      } else {
```

Line 90: Else (not the middle row of empty file).

```c
        abAppend(ab, "~", 1);
```

Line 91: Draw a tilde (~) to indicate empty row (like vim).

```c
      }
```

Line 92: Closes the inner if-else.

```c
    } else {
```

Line 93: Else (filerow < numrows, there's a row to display).

```c
      int len = E.row[filerow].size;
      if (len > E.screenCols)
        abAppend(ab, E.row[filerow].chars, len);
```

Lines 94-95: Get the row length and append row content.

```c
    }
```

Line 96: Closes the if-else for row display.

```c
    abAppend(ab, "\x1b[K", 3);
```

Line 97: Clear from cursor to end of line (`\x1b[K` = EL - Erase Line).

```c
    if (y < E.screenRows - 1) {
      abAppend(ab, "\r\n", 2);
    }
```

Lines 98-100: Add newline (except for last row).

```c
  }
}
```

Lines 101-102: Closes the for loop and editorDrawRows function.

```c
void editorRefreshScreen() {
```

Line 103: Main screen refresh function - clears and redraws everything.

```c
  editorScroll();
```

Line 104: Calculate scroll offset before drawing.

```c
  struct abuf ab = ABUF_INIT;
```

Line 105: Initialize an empty append buffer.

```c
  abAppend(&ab, "\x1b[H", 3);
```

Line 106: Move cursor to home position (ANSI escape).

```c
  abAppend(&ab, "\x1b[?25l", 6);
```

Line 107: Hide cursor during refresh (ANSI escape `\x1b[?25l` = hide cursor).

```c
  editorDrawRows(&ab);
```

Line 108: Draw all rows to the append buffer.

```c
  char buf[32];
```

Line 109: Buffer for cursor position string.

```c
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.rowoff) + 1, E.cx + 1);
```

Line 110: Format cursor position command, accounting for scroll offset. ANSI uses 1-indexed positions, so we add 1.

```c
  abAppend(&ab, buf, strlen(buf));
```

Line 111: Append the cursor position command to buffer.

```c
  abAppend(&ab, "\x1b[?25h", 6);
```

Line 112: Show cursor again (`\x1b[?25h` = show cursor).

```c
  write(STDOUT_FILENO, ab.buf, ab.len);
```

Line 113: Write the entire buffer to the terminal in one operation.

```c
  abFree(&ab);
```

Line 114: Free the buffer memory.

```c
}
```

Line 115: Closes editorRefreshScreen function.

```c
char editorReadKey() {
```

Line 116: Reads a keypress, handling escape sequences for special keys.

```c
  char c;
  int nread;
```

Lines 117-118: Variables for character and read return value.

```c
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN)
      die("read");
  }
```

Lines 119-122: Loop until we read exactly 1 character. If read returns -1 and it's not EAGAIN (try again), it's an error.

```c
  if ('\x1b' == c) {
```

Line 123: If the character is ESC (escape), it's the start of an escape sequence.

```c
    char seq[3];
```

Line 124: Buffer for escape sequence characters.

```c
    if (read(STDIN_FILENO, &seq[0], 1) != 1)
      return '\x1b';
```

Lines 125-126: Read next character. If fail, return plain ESC.

```c
    if (read(STDIN_FILENO, &seq[1], 1) != 1)
      return '\x1b';
```

Lines 127-128: Read another character for the sequence.

```c
    if (seq[0] == '[') {
```

Line 129: If second char is '[', it's an CSI (Control Sequence Introducer) sequence.

```c
      if (seq[1] >= '0' && seq[1] <= '9') {
```

Line 130: If third character is a digit, it's an extended sequence (like ~ sequences).

```c
        if (read(STDIN_FILENO, &seq[2], 1) != 1)
          return '\x1b';
```

Lines 131-132: Read the fourth character.

```c
        if (seq[2] == '~') {
```

Line 133: If fourth character is '~', it's a function key sequence.

```c
          switch (seq[1]) {
```

Line 134: Switch on the digit character.

```c
          case '1':
            return HOME_KEY;
```

Line 136: '1' + '~' = Home key.

```c
          case '3':
            return DEL_KEY;
```

Line 138: '3' + '~' = Delete key.

```c
          case '4':
            return END_KEY;
```

Line 140: '4' + '~' = End key.

```c
          case '5':
            return PAGE_UP;
```

Line 142: '5' + '~' = Page Up.

```c
          case '6':
            return PAGE_DOWN;
```

Line 144: '6' + '~' = Page Down.

```c
          case '7':
            return HOME_KEY;
```

Line 146: '7' + '~' = Home key (alternative).

```c
          case '8':
            return END_KEY;
```

Line 148: '8' + '~' = End key (alternative).

```c
          }
```

Line 149: Closes the inner switch.

```c
        }
```

Line 150: Closes the '~' check.

```c
      } else {
```

Line 151: Else (third char is not a digit).

```c
        switch (seq[1]) {
```

Line 152: Switch on the third character.

```c
        case 'A':
          return ARROW_UP;
```

Line 154: '[' + 'A' = Arrow Up.

```c
        case 'B':
          return ARROW_DOWN;
```

Line 156: '[' + 'B' = Arrow Down.

```c
        case 'C':
          return ARROW_RIGHT;
```

Line 158: '[' + 'C' = Arrow Right.

```c
        case 'D':
          return ARROW_LEFT;
```

Line 160: '[' + 'D' = Arrow Left.

```c
        case 'H':
          return HOME_KEY;
```

Line 162: '[' + 'H' = Home key (alternative).

```c
        case 'F':
          return END_KEY;
```

Line 164: '[' + 'F' = End key (alternative).

```c
        }
```

Line 165: Closes the switch.

```c
      }
```

Line 166: Closes the else branch.

```c
    } else if (seq[0] == 'O') {
```

Line 167: If second character is 'O', it's an SS3 (Single Shift 3) sequence.

```c
      switch (seq[1]) {
```

Line 168: Switch on third character.

```c
      case 'H':
        return HOME_KEY;
```

Line 170: 'O' + 'H' = Home key.

```c
      case 'F':
        return END_KEY;
```

Line 172: 'O' + 'F' = End key.

```c
      }
```

Line 173: Closes the switch.

```c
    }
```

Line 174: Closes the sequence type check.

```c
    return '\x1b';
```

Line 175: Return plain ESC for unknown sequences.

```c
  } else {
```

Line 176: Else (not an escape sequence - regular key).

```c
    return c;
```

Line 177: Return the character as-is.

```c
  }
}
```

Lines 178-179: Closes the editorReadKey function.

```c
void editorProcessKeypress() {
```

Line 180: Main key processing function - handles all key inputs based on mode.

```c
  char c = editorReadKey();
```

Line 181: Read the key that was pressed.

```c
  if (E.mode == NORMAL_MODE) {
```

Line 182: If in normal mode.

```c
    switch (c) {
```

Line 183: Switch on the key character.

```c
    case CTRL_KEY('q'):
```

Line 184: If Ctrl+Q pressed.

```c
      write(STDOUT_FILENO, "\x1b[?25h", 6);
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
```

Lines 185-187: Show cursor, clear screen, move to home.

```c
      exit(0);
```

Line 188: Exit the program successfully.

```c
      break;
```

Line 189: Break (unreachable after exit).

```c
    case HOME_KEY:
```

Line 190: Home key case.

```c
      E.cx = 0;
```

Line 191: Move cursor to column 0.

```c
      break;
```

Line 192: Break.

```c
    case END_KEY:
```

Line 193: End key case.

```c
      E.cx = E.screenCols - 1;
```

Line 194: Move cursor to last column.

```c
      break;
```

Line 195: Break.

```c
    case PAGE_UP:
    case PAGE_DOWN: {
```

Lines 196-197: Page Up/Down case.

```c
      int times = E.screenRows;
      while (times--) {
        editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
      }
```

Lines 198-201: Move cursor up/down by screen rows (simulating page scroll).

```c
    } break;
```

Line 202: Break out of case.

```c
    case ARROW_LEFT:
    case ARROW_DOWN:
    case ARROW_UP:
    case ARROW_RIGHT:
```

Lines 203-206: Arrow key cases.

```c
      editorMoveCursor(c);
```

Line 207: Call move cursor function.

```c
      break;
```

Line 208: Break.

```c
    case 'h':
```

Line 209: Vim-style 'h' for left.

```c
      editorMoveCursor(ARROW_LEFT);
```

Line 210: Move left.

```c
      break;
```

Line 211: Break.

```c
    case 'j':
```

Line 212: Vim-style 'j' for down.

```c
      editorMoveCursor(ARROW_DOWN);
```

Line 213: Move down.

```c
      break;
```

Line 214: Break.

```c
    case 'k':
```

Line 215: Vim-style 'k' for up.

```c
      editorMoveCursor(ARROW_UP);
```

Line 216: Move up.

```c
      break;
```

Line 217: Break.

```c
    case 'l':
```

Line 218: Vim-style 'l' for right.

```c
      editorMoveCursor(ARROW_RIGHT);
```

Line 219: Move right.

```c
      break;
```

Line 220: Break.

```c
    case 'i':
```

Line 221: 'i' key for insert mode.

```c
      E.mode = INSERT_MODE;
```

Line 222: Switch to insert mode.

```c
      break;
```

Line 223: Break.

```c
    case 'a':
```

Line 224: 'a' key for append (insert after cursor).

```c
      E.cx++;
```

Line 225: Move cursor one position right.

```c
      E.mode = INSERT_MODE;
```

Line 226: Switch to insert mode.

```c
      break;
```

Line 227: Break.

```c
    default:
```

Line 228: Default case for other keys.

```c
      if (c >= 32 && c <= 126) {
        E.cx++;
      }
```

Lines 229-231: If printable ASCII (space through tilde), just move cursor right.

```c
    }
```

Line 232: Closes the normal mode switch.

```c
  } else if (INSERT_MODE == E.mode) {
```

Line 233: Else if in insert mode.

```c
    switch (c) {
```

Line 234: Switch on key.

```c
    case '\x1b':
```

Line 235: Escape key case.

```c
      E.mode = NORMAL_MODE;
```

Line 236: Return to normal mode.

```c
      break;
```

Line 237: Break.

```c
    default:
```

Line 238: Default case.

```c
      if (c >= 32 && c <= 126) {
        E.cx++;
      }
```

Lines 239-241: If printable character, move cursor right (text insertion not fully implemented).

```c
    }
```

Line 242: Closes the insert mode switch.

```c
  }
}
```

Lines 243-244: Closes editorProcessKeypress function.

```c
void initEditor() {
```

Line 245: Editor initialization function.

```c
  E.cx = 0;
  E.cy = 0;
```

Lines 246-247: Reset cursor position to top-left.

```c
  E.rowoff = 0;
```

Line 248: Initialize scroll offset to 0.

```c
  E.numrows = 0;
```

Line 249: No rows loaded initially.

```c
  E.row = NULL;
```

Line 250: Row pointer initially NULL (no memory allocated).

```c
  E.mode = NORMAL_MODE;
```

Line 251: Start in normal mode.

```c
  if (getWindowsize(&E.screenRows, &E.screenCols) == -1)
    die("getWindowsize");
}
```

Lines 252-253: Get terminal window size. If it fails, call die.

---

## Summary

This is a minimal terminal-based text editor inspired by vim and the Kilo tutorial. It features:

- **Raw terminal mode** for character-by-character input
- **Two modes**: Normal (vim-like navigation with h/j/k/l/arrows) and Insert mode
- **Multi-line file support**: Opens and displays multiple lines from a file
- **Scrolling support**: Vertical scrolling when cursor moves beyond visible screen
- **Command-line arguments**: Accepts filename as command-line argument
- **ANSI escape sequence handling**: For cursor movement, screen clearing, and special keys
- **Append buffer**: Efficient string building for screen updates

The editor now supports opening files with multiple lines and proper vertical scrolling.
