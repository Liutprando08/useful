# Piano di Migrazione: Da E.row a Piece Table

<!--toc:start-->
- [Piano di Migrazione: Da E.row a Piece Table](#piano-di-migrazione-da-erow-a-piece-table)
  - [Panoramica](#panoramica)
  - [Struttura Attuale](#struttura-attuale)
    - [struct erow (da rimuovere)](#struct-erow-da-rimuovere)
    - [struct editorConfig (modifiche)](#struct-editorconfig-modifiche)
  - [Struttura Dati Finale](#struttura-dati-finale)
    - [line_offsets](#lineoffsets)
    - [row_cache](#rowcache)
  - [Funzioni Nuove](#funzioni-nuove)
    - [1. initLineOffsets()](#1-initlineoffsets)
    - [2. invalidateCacheFrom(int row)](#2-invalidatecachefromint-row)
    - [3. editorGetColumn()](#3-editorgetcolumn)
    - [4. editorGetRowContent()](#4-editorgetrowcontent)
    - [5. editorGetRenderedRow()](#5-editorgetrenderedrow)
  - [Modifiche per File](#modifiche-per-file)
    - [1. include/set_terminal.h](#1-includesetterminalh)
    - [2. src/viewer.c](#2-srcviewerc)
    - [3. src/draw.c](#3-srcdrawc)
    - [4. src/text.c](#4-srctextc)
    - [5. src/editorGraphic.c](#5-srceditorgraphicc)
  - [Operazioni sulla Piece Table](#operazioni-sulla-piece-table)
    - [Inserimento Carattere](#inserimento-carattere)
    - [Cancellazione Carattere](#cancellazione-carattere)
    - [Cancellazione Riga](#cancellazione-riga)
  - [Gestione della Cache](#gestione-della-cache)
    - [Quando Invalidare](#quando-invalidare)
    - [Gestione Memoria](#gestione-memoria)
  - [Vantaggi dell'Approccio](#vantaggi-dellapproccio)
  - [TODO](#todo)
<!--toc:end-->

## Panoramica

Questo documento descrive come migrare l'editor da `E.row` (array di struct `erow`) a una struttura basata su piece table con cache per il rendering.

---

## Struttura Attuale

### struct erow (da rimuovere)

```c
typedef struct erow {
    int size;        // lunghezza riga in caratteri
    int rsize;       // lunghezza renderizzata (con tab espansi)
    char *chars;     // contenuto riga
    char *render;    // contenuto renderizzato
} erow;
```

### struct editorConfig (modifiche)

```c
struct editorConfig {
    // ... campi esistenti ...
    
    // NUOVO: offset di inizio ogni riga
    int *line_offsets;           // array di offset (dimensione numrows + 1)
    int line_offsets_capacity;   // capacità array
    
    // NUOVO: cache per rendering
    char **row_cache;         // array di stringhe renderizzate
    int *row_cache_rsize;     // lunghezza renderizzata per ogni riga
    int row_cache_valid;      // flag: 1 = cache valido, 0 = rigenerare
};
```

---

## Struttura Dati Finale

### line_offsets

Array che memorizza l'offset (in caratteri) di inizio di ogni riga nel buffer totale:

```
Esempio con 3 righe:
"Hello\nWorld\nTest"

line_offsets[0] = 0   // inizio riga 0
line_offsets[1] = 6  // inizio riga 1 (dopo "Hello\n")
line_offsets[2] = 12 // inizio riga 2 (dopo "World\n")
line_offsets[3] = 17 // fine file (dopo "Test")
```

### row_cache

Cache per il rendering delle righe (con tab expansion). Ogni entry contiene la versione renderizzata della riga (con tab convertiti in spazi).

---

## Funzioni Nuove

### 1. initLineOffsets()

Costruisce l'array `line_offsets` dalla piece table.

```c
void initLineOffsets() {
    free(E.line_offsets);
    E.line_offsets = malloc(sizeof(int) * (E.numrows + 2));
    E.line_offsets[0] = 0;
    
    int line = 1;
    for (int i = 0; i < T.original_length; i++) {
        if (T.original_buffer[i] == '\n') {
            E.line_offsets[line++] = i + 1;
        }
    }
    E.line_offsets[E.numrows + 1] = T.original_length;
    E.line_offsets_capacity = E.numrows + 2;
}
```

### 2. invalidateCacheFrom(int row)

Invalida la cache del rendering a partire da una determinata riga.

```c
void invalidateCacheFrom(int row) {
    if (!E.row_cache) return;
    for (int i = row; i < E.numrows; i++) {
        free(E.row_cache[i]);
        E.row_cache[i] = NULL;
    }
    E.row_cache_valid = 0;
}
```

### 3. editorGetColumn()

Calcola la colonna corrente (caratteri dall'inizio della riga).

```c
int editorGetColumn() {
    if (E.cy < 0 || E.cy >= E.numrows) return 0;
    return E.cx - E.line_offsets[E.cy];
}
```

### 4. editorGetRowContent()

Legge il contenuto di una riga dalla piece table.

```c
int editorGetRowContent(int row, char *buf, int bufsize) {
    if (row < 0 || row >= E.numrows) return -1;
    
    int start = E.line_offsets[row];
    int end = E.line_offsets[row + 1] - 1; // esclude \n
    int len = end - start;
    
    if (len >= bufsize) len = bufsize - 1;
    
    // Itera sulle piece per copiare i caratteri
    int buf_pos = 0;
    int current_pos = 0;
    
    for (int i = 0; i < T.pieces_count && buf_pos < len; i++) {
        piece *p = &T.pieces[i];
        int piece_start = current_pos;
        int piece_end = current_pos + p->length;
        
        // Se la piece si sovrappone alla riga
        if (piece_end > start && piece_start < end) {
            char *src = (p->buffer == BUFFER_ORIGINAL) ? 
                        T.original_buffer : T.add_buffer;
            
            int copy_start = (piece_start < start) ? start - piece_start : 0;
            int copy_end = (piece_end > end) ? end - piece_start : p->length;
            
            memcpy(buf + buf_pos, src + p->start + copy_start, 
                   copy_end - copy_start);
            buf_pos += copy_end - copy_start;
        }
        current_pos = piece_end;
    }
    
    buf[buf_pos] = '\0';
    return buf_pos;
}
```

### 5. editorGetRenderedRow()

Ottiene o calcola la versione renderizzata di una riga.

```c
char *editorGetRenderedRow(int row) {
    if (row < 0 || row >= E.numrows) return NULL;
    
    // Alloca cache se necessario
    if (!E.row_cache) {
        E.row_cache = calloc(E.numrows, sizeof(char*));
        E.row_cache_rsize = calloc(E.numrows, sizeof(int));
    }
    
    // Se già in cache, restituisci
    if (E.row_cache[row]) {
        return E.row_cache[row];
    }
    
    // Altrimenti, calcola
    int start = E.line_offsets[row];
    int end = E.line_offsets[row + 1] - 1;
    int size = end - start;
    
    // Leggi contenuto riga
    char *row_content = malloc(size + 1);
    editorGetRowContent(row, row_content, size + 1);
    
    // Conta tab per dimensione renderizzata
    int render_size = size;
    for (int i = 0; i < size; i++) {
        if (row_content[i] == '\t') {
            render_size += KILO_TAB_STOP - 1;
        }
    }
    
    // Allocazione buffer renderizzato
    char *rendered = malloc(render_size + 1);
    int idx = 0;
    for (int i = 0; i < size; i++) {
        if (row_content[i] == '\t') {
            rendered[idx++] = ' ';
            while (idx % KILO_TAB_STOP != 0)
                rendered[idx++] = ' ';
        } else {
            rendered[idx++] = row_content[i];
        }
    }
    rendered[idx] = '\0';
    
    free(row_content);
    
    E.row_cache[row] = rendered;
    E.row_cache_rsize[row] = idx;
    
    return rendered;
}
```

---

## Modifiche per File

### 1. include/set_terminal.h

**Aggiungere alla struct editorConfig:**

```c
int *line_offsets;
int line_offsets_capacity;
char **row_cache;
int *row_cache_rsize;
int row_cache_valid;
```

**Aggiungere prototipi:**

```c
void initLineOffsets();
void invalidateCacheFrom(int row);
int editorGetColumn();
int editorGetAbsolutePosition();
int editorGetRowContent(int row, char *buf, int bufsize);
char *editorGetRenderedRow(int row);
```

**Rimuovere:**

- `void editorUpdateRow(erow *row);`
- `void editorRowInsertChar(erow *row, int at, int c);`
- `void editorRowDelChar(erow *row, int at);`
- `void editorFreeRow(erow *row);`
- `void editorRowAppendString(erow *row, char *s, size_t len);`

### 2. src/viewer.c

- Modificare `editorOpen()` per chiamare `initLineOffsets()`
- Implementare `initLineOffsets()`
- Implementare `invalidateCacheFrom()`
- Implementare `editorGetColumn()`
- Implementare `editorGetAbsolutePosition()` (che coincide con `E.cx`)
- Implementare `editorGetRowContent()`
- Implementare `editorGetRenderedRow()`

**Rimuovere:**

- `editorRowCxToRx()` - sostituire con logica in `editorGetRenderedRow()`
- `editorUpdateRow()` - non più necessario
- `editorRowAppendString()` - non più necessario

### 3. src/draw.c

- Modificare `editorDrawRows()` per usare `editorGetRenderedRow()`
- Modificare `editorDrawStatusBar()` per mostrare riga, colonna e posizione assoluta

**Prima:**

```c
int len = E.row[filerow].rsize - E.coloff;
abAppend(ab, &E.row[filerow].render[E.coloff], len);
```

**Dopo:**

```c
char *rendered = editorGetRenderedRow(filerow);
int len = E.row_cache_rsize[filerow] - E.coloff;
if (len < 0) len = 0;
if (len > E.screenCols) len = E.screenCols;
abAppend(ab, &rendered[E.coloff], len);
```

**Status bar:**

```c
int col = editorGetColumn();
int rlen = snprintf(rstatus, sizeof(rstatus), 
                    "%d/%d | col %d | abs %d",
                    E.cy + 1, E.numrows + 1, col + 1, E.cx + 1);
```

### 4. src/text.c

- Riscrivere `editorRowsToString()` per usare la piece table
- Riscrivere `editorSave()` per usare la piece table
- Modificare `editorInsertChar()` per usare `piece_table_insert()`
- Modificare `editorDelChar()` per aggiornare `line_offsets`
- Modificare `editorInsertNewline()` per aggiornare `line_offsets` e invalidare cache
- Modificare `editorDelRow()` per aggiornare `line_offsets` e invalidare cache

**Per editorInsertNewline():**

```c
void editorInsertNewline() {
    // Verifica che ci sia spazio nell'array
    if (E.numrows + 2 > E.line_offsets_capacity) {
        E.line_offsets_capacity *= 2;
        E.line_offsets = realloc(E.line_offsets, 
                    sizeof(int) * E.line_offsets_capacity);
    }
    
    // Sposta gli offset dopo il cursore
    for (int i = E.numrows; i > E.cy; i--) {
        E.line_offsets[i + 1] = E.line_offsets[i];
    }
    E.line_offsets[E.cy + 1] = E.cx + 1;
    
    E.numrows++;
    E.cy++;
    E.cx = 0;
    
    invalidateCacheFrom(E.cy);
    E.dirty++;
}
```

**Per editorDelRow():**

```c
void editorDelRow(int at) {
    if (at < 0 || at >= E.numrows) return;
    
    // Rimuovi l'offset della riga eliminata
    for (int i = at; i < E.numrows; i++) {
        E.line_offsets[i] = E.line_offsets[i + 1];
    }
    E.numrows--;
    
    invalidateCacheFrom(at);
    E.dirty++;
}
```

### 5. src/editorGraphic.c

- In `initEditor()`, inizializzare i nuovi campi a NULL/0
- Modificare i controlli per `E.row[E.cy].size` per usare calcoli da `line_offsets`

**Prima:**

```c
if (E.cx > E.row[E.cy].size)
    E.cx = E.row[E.cy].size;
```

**Dopo:**

```c
if (E.cy < E.numrows) {
    int row_size = E.line_offsets[E.cy + 1] - E.line_offsets[E.cy] - 1;
    if (E.cx > row_size) E.cx = row_size;
}
```

---

## Operazioni sulla Piece Table

### Inserimento Carattere

1. Chiama `piece_table_insert(char *c)` per inserire nella piece table
2. Incrementa `E.cx`
3. Aggiorna gli offset dopo il cursore (+1)
4. Invalida cache da `E.cy`

```c
void editorInsertChar(int c) {
    char buf[2] = {c, '\0'};
    piece_table_insert(buf);
    
    // Aggiorna line_offsets
    for (int i = E.cy + 1; i <= E.numrows; i++) {
        E.line_offsets[i]++;
    }
    
    E.cx++;
    invalidateCacheFrom(E.cy);
    E.dirty++;
}
```

### Cancellazione Carattere

1. Usa la piece table per eliminare il carattere
2. Aggiorna `E.cx`
3. Aggiorna gli offset dopo il cursore (-1)
4. Invalida cache

### Cancellazione Riga

1. Calcola la lunghezza della riga da eliminare
2. Usa la piece table per eliminare i caratteri
3. Usa `memmove` per rimuovere l'offset
4. Aggiorna `E.numrows`
5. Invalida cache

---

## Gestione della Cache

### Quando Invalidare

- Dopo `editorInsertChar()`
- Dopo `editorDelChar()`
- Dopo `editorInsertNewline()`
- Dopo `editorDelRow()`
- Dopo qualsiasi modifica tramite piece table

### Gestione Memoria

In `die()` o alla chiusura dell'editor:

```c
free(E.line_offsets);
if (E.row_cache) {
    for (int i = 0; i < E.numrows; i++) {
        free(E.row_cache[i]);
    }
    free(E.row_cache);
}
free(E.row_cache_rsize);
```

---

## Vantaggi dell'Approccio

| Aspetto | Descrizione |
|---------|-------------|
| Performance | Cache O(1) per rendering, lookup O(1) per riga/colonna |
| Memoria | Cache solo per righe visibili + necessarie |
| Semplicità | Piece table come fonte di verità, offset come indice |
| Manutenzione | Logica centralizzata, invalidazione selettiva |

---

## TODO

- [ ] Implementare `initLineOffsets()` in `src/viewer.c`
- [ ] Implementare `invalidateCacheFrom()` in `src/viewer.c`
- [ ] Implementare `editorGetColumn()` in `src/viewer.c`
- [ ] Implementare `editorGetAbsolutePosition()` in `src/viewer.c`
- [ ] Implementare `editorGetRowContent()` in `src/viewer.c`
- [ ] Implementare `editorGetRenderedRow()` in `src/viewer.c`
- [ ] Aggiornare `src/draw.c` per usare la nuova API
- [ ] Aggiornare `src/text.c` per usare piece table + line_offsets
- [ ] Aggiornare `src/editorGraphic.c` per rimuovere dipendenze da `E.row`
- [ ] Aggiornare `include/set_terminal.h` con nuovi campi e prototipi
- [ ] Testare thoroughly ogni funzionalità
