#include "editor.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

int getCursorPosition(int *rows, int *cols) {
  char buf[32];
  unsigned int i = 0;
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
    return -1;
  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &buf[i], 1) != 1)
      break;
    if (buf[i] == 'R')
      break;
    i++;
  }
  buf[i] = '\0';
  if (buf[0] != '\x1b' || buf[1] != '[')
    return -1;
  if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
    return -1;
  return 0;
}

char *getActualLineContent() {
  if (E.cy < 0 || E.cy >= E.numrows)
    return NULL;

  int row_size = E.line_offsets[E.cy + 1] - E.line_offsets[E.cy];
  char *content = malloc(row_size + 1);
  if (!content)
    return NULL;

  editorGetRowContent(E.cy, content, row_size + 1);
  return content;
}

int renderedPosToActualPos(int renderCol) {
  char *content = getActualLineContent();
  if (!content)
    return 0;

  int actualCol = 0;
  int renderPos = 0;

  while (actualCol < (int)strlen(content) && renderPos < renderCol) {
    if (content[actualCol] == '\t') {

      renderPos++;
      while (renderPos % KILO_TAB_STOP != 0)
        renderPos++;
    } else {
      renderPos++;
    }
    actualCol++;
  }

  free(content);
  return actualCol;
}

char getCharAtRenderedPos(int renderCol) {
  char *content = getActualLineContent();
  if (!content)
    return '\0';

  int actualCol = 0;
  int renderPos = 0;

  while (actualCol < (int)strlen(content)) {
    int char_render_start = renderPos;
    int char_render_end;

    if (content[actualCol] == '\t') {
      char_render_end = renderPos;
      while ((char_render_end + 1) % KILO_TAB_STOP != 0)
        char_render_end++;
    } else {
      char_render_end = renderPos;
    }

    if (renderCol >= char_render_start && renderCol <= char_render_end) {
      char result = content[actualCol];
      free(content);
      return result;
    }

    if (content[actualCol] == '\t') {
      renderPos++;
      while (renderPos % KILO_TAB_STOP != 0)
        renderPos++;
    } else {
      renderPos++;
    }

    actualCol++;
  }

  free(content);
  return '\0';
}

int skipConsecutiveTabs(int actualCol) {
  char *content = getActualLineContent();
  if (!content)
    return E.cx;

  int contentLen = strlen(content);
  int renderCol = 0;

  for (int i = 0; i < actualCol && i < contentLen; i++) {
    if (content[i] == '\t') {
      renderCol++;
      while (renderCol % KILO_TAB_STOP != 0)
        renderCol++;
    } else {
      renderCol++;
    }
  }

  int tempActual = actualCol;
  while (tempActual < contentLen && content[tempActual] == '\t') {
    renderCol++;
    while (renderCol % KILO_TAB_STOP != 0)
      renderCol++;
    tempActual++;
  }

  free(content);
  return renderCol;
}

void editorMoveCursor(char key) {
  switch (key) {
  case ARROW_LEFT: {
    if (E.cx > 0) {
      E.cx--;

      int actualCol = renderedPosToActualPos(E.cx);
      char *content = getActualLineContent();

      if (content && actualCol > 0 && content[actualCol - 1] == '\t') {

        int tabStart = actualCol - 1;
        while (tabStart > 0 && content[tabStart - 1] == '\t') {
          tabStart--;
        }

        int renderPos = 0;
        for (int i = 0; i < tabStart; i++) {
          if (content[i] == '\t') {
            renderPos++;
            while (renderPos % KILO_TAB_STOP != 0)
              renderPos++;
          } else {
            renderPos++;
          }
        }
        E.cx = renderPos;
      }

      free(content);
    } else if (E.cx == 0) {
      if (E.cy > 0) {
        E.cy--;
        E.cx = E.row_cache_rsize[E.cy] - 1;
      }
    }
    break;
  }
  case ARROW_RIGHT: {
    if (!E.row_cache[E.cy])
      break;

    int lineRenderLen = E.row_cache_rsize[E.cy] - 1;

    if (E.cx >= lineRenderLen) {
      if (E.cy < E.numrows - 1) {
        E.cy++;
        E.cx = 0;
      } else {
        break;
      }

    } else {

      E.cx++;
      int actualCol = renderedPosToActualPos(E.cx);
      char *content = getActualLineContent();

      lineRenderLen = E.row_cache[E.cy] ? E.row_cache_rsize[E.cy] : 0;

      if (content && actualCol < (int)strlen(content) &&
          content[actualCol] == '\t') {
        E.cx = skipConsecutiveTabs(actualCol);

        if (E.cx > lineRenderLen)
          E.cx = lineRenderLen;
      }

      free(content);
    }

    break;
  }
  case ARROW_DOWN:
    if (E.cy < E.numrows - 1) {
      E.cy++;
      E.cx = 0;
    }

    int actualCol = renderedPosToActualPos(E.cx);
    char *content = getActualLineContent();
    if (content && actualCol < (int)strlen(content) &&
        content[actualCol] == '\t') {
      E.cx = skipConsecutiveTabs(actualCol);
    }

    free(content);
    break;
  case ARROW_UP:
    if (E.cy > 0) {
      E.cy--;
      E.cx = 0;
    }

    actualCol = renderedPosToActualPos(E.cx);
    content = getActualLineContent();
    if (content && actualCol < (int)strlen(content) &&
        content[actualCol] == '\t') {
      E.cx = skipConsecutiveTabs(actualCol);
    }

    free(content);
    break;
  }
}
