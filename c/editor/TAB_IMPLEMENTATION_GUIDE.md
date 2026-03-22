# Manual Implementation Guide: Tab Counting & Skipping

## Understanding the Current System

Your editor works like this:
- **`row_content`** = actual text with real tab characters (`\t`)
- **`rendered`** = display version where each `\t` becomes spaces (lines 42-45 in `cache.c`)
- **`E.cx`** = cursor position in rendered coordinates (what you see on screen)
- **`E.row_cache_rsize[row]`** = length of rendered line

**Key insight**: A single `\t` in the actual text becomes 1-8 spaces in rendering depending on position.

---

## Implementation Steps

### **Step 1: Create Helper Function to Get Actual Line Content**

In **`src/cursor.c`**, add this function before `editorMoveCursor()`:

```c
// Get the raw (unrendered) content of current line
// Returns dynamically allocated string - caller must free
char *getActualLineContent() {
  if (E.cy < 0 || E.cy >= E.numrows)
    return NULL;
  
  int row_size = E.line_offsets[E.cy + 1] - E.line_offsets[E.cy];
  char *content = malloc(row_size + 1);
  if (!content) return NULL;
  
  editorGetRowContent(E.cy, content, row_size + 1);
  return content;
}
```

---

### **Step 2: Create Function to Convert Rendered Position to Actual Position**

In **`src/cursor.c`**, add this before `editorMoveCursor()`:

```c
// Convert rendered column position to actual (tab-aware) column position
// Returns the index in the actual (non-rendered) line content
int renderedPosToActualPos(int renderCol) {
  char *content = getActualLineContent();
  if (!content)
    return 0;
  
  int actualCol = 0;  // position in actual content
  int renderPos = 0;  // position in rendered output
  
  // Iterate through actual content until we reach the render position
  while (actualCol < strlen(content) && renderPos < renderCol) {
    if (content[actualCol] == '\t') {
      // Tab expands to next tab stop
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
```

---

### **Step 3: Create Function to Skip Consecutive Tabs**

In **`src/cursor.c`**, add this before `editorMoveCursor()`:

```c
// Get the render column position right after all consecutive tabs
// from given actualCol position
int skipConsecutiveTabs(int actualCol) {
  char *content = getActualLineContent();
  if (!content)
    return E.cx;
  
  int contentLen = strlen(content);
  int renderCol = 0;
  
  // First, find render position of actualCol
  for (int i = 0; i < actualCol && i < contentLen; i++) {
    if (content[i] == '\t') {
      renderCol++;
      while (renderCol % KILO_TAB_STOP != 0)
        renderCol++;
    } else {
      renderCol++;
    }
  }
  
  // Now skip all consecutive tabs from actualCol
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
```

---

### **Step 4: Modify `editorMoveCursor()` for ARROW_RIGHT**

Replace the `ARROW_RIGHT` case in `editorMoveCursor()` (lines 41-50) with:

```c
case ARROW_RIGHT: {
  if (!E.row_cache[E.cy])
    break;
  
  int lineRenderLen = E.row_cache_rsize[E.cy];
  
  // If at end of line, go to next line
  if (E.cx >= lineRenderLen) {
    if (E.cy < E.numrows - 1) {
      E.cy++;
      E.cx = 0;
    }
    break;
  }
  
  // Move cursor right by 1
  E.cx++;
  
  // Check if we landed on tabs and skip them
  int actualCol = renderedPosToActualPos(E.cx);
  char *content = getActualLineContent();
  
  if (content && actualCol < strlen(content) && content[actualCol] == '\t') {
    // We're at the start of one or more tabs - skip them all
    E.cx = skipConsecutiveTabs(actualCol);
    
    // Ensure we don't exceed line length
    if (E.cx > lineRenderLen)
      E.cx = lineRenderLen;
  }
  
  free(content);
  break;
}
```

---

### **Step 5: Modify `editorMoveCursor()` for ARROW_LEFT**

Replace the `ARROW_LEFT` case in `editorMoveCursor()` (lines 31-40) with:

```c
case ARROW_LEFT: {
  if (E.cx > 0) {
    E.cx--;
    
    // Check if we landed on tabs
    int actualCol = renderedPosToActualPos(E.cx);
    char *content = getActualLineContent();
    
    if (content && actualCol > 0 && content[actualCol - 1] == '\t') {
      // We landed after a tab - find the start of the tab sequence
      int tabStart = actualCol - 1;
      while (tabStart > 0 && content[tabStart - 1] == '\t') {
        tabStart--;
      }
      
      // Convert tab start position back to rendered position
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
      E.cx = E.row_cache_rsize[E.cy];
    }
  }
  break;
}
```

---

### **Step 6: Clean Up Existing Tab Handling**

**Delete lines 62-69** in the current `editorMoveCursor()` function:

```c
// DELETE THIS:
int rowlen = E.row_cache[E.cy] ? E.row_cache_rsize[E.cy] - 1 : 0;
if (E.cx > rowlen) {
  E.cx = rowlen;

  if (E.row_cache[E.cy][E.cx] == '\t') {
    E.cx += KILO_TAB_STOP;
  }
}
```

Replace with just:

```c
// Ensure cursor doesn't exceed line bounds
int rowlen = E.row_cache[E.cy] ? E.row_cache_rsize[E.cy] : 0;
if (E.cx > rowlen)
  E.cx = rowlen;
```

---

### **Step 7: Update Header File**

In **`include/editor.h`**, add these function prototypes after the existing ones:

```c
// Tab-aware cursor helpers
char *getActualLineContent();
int renderedPosToActualPos(int renderCol);
int skipConsecutiveTabs(int actualCol);
```

---

## Summary of Changes

| File | Change |
|------|--------|
| `src/cursor.c` | Add 3 helper functions, rewrite ARROW_RIGHT and ARROW_LEFT cases, remove old tab handling (lines 62-69) |
| `include/editor.h` | Add 3 function prototypes |

---

## How It Works

1. **When pressing RIGHT**: 
   - Move cursor right by 1
   - Check if landed on actual tab character
   - If yes, skip all consecutive tabs to position after them
   - Never exceed line bounds

2. **When pressing LEFT**:
   - Move cursor left by 1  
   - Check if previous actual character was tab
   - If yes, skip back to start of tab sequence
   - Handle line wrapping

3. **Multi-tab handling**:
   - `skipConsecutiveTabs()` counts forward through all `\t` chars
   - Calculates rendered width for each tab (1-8 spaces)
   - Returns final rendered position after all tabs

---

## Testing Checklist

- [ ] Create test file with tabs
- [ ] Move RIGHT through single tab - should skip to next character
- [ ] Move RIGHT through multiple consecutive tabs - should skip all
- [ ] Move LEFT back through tabs - should go to tab start
- [ ] Move RIGHT at end of line - should go to next line
- [ ] Move LEFT at line start - should go to previous line end
- [ ] Mix tabs with regular text - cursor should skip correctly
