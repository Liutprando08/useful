#ifndef CURSOR_H
#define CURSOR_H

int renderedPosToActualPos(int renderCol);

char *getActualLineContent();

int skipConsecutiveTabs(int actualCol);

char getCharAtRenderedPos(int renderCol);

#endif
