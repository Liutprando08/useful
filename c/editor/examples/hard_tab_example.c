#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAB_STOP 8

typedef enum { BUFFER_ORIGINAL, BUFFER_ADD } BufferType;

typedef struct {
    BufferType buffer;
    int start;
    int length;
} Piece;

typedef struct {
    char *original_buffer;
    size_t original_length;
    char *add_buffer;
    int add_capacity;
    int add_length;
    Piece *pieces;
    int pieces_count;
    int pieces_capacity;
} PieceTable;

void pt_init(PieceTable *pt) {
    pt->original_buffer = NULL;
    pt->original_length = 0;
    pt->add_capacity = 1024;
    pt->add_buffer = malloc(pt->add_capacity);
    pt->add_length = 0;
    pt->pieces_capacity = 100;
    pt->pieces = malloc(sizeof(Piece) * pt->pieces_capacity);
    pt->pieces_count = 0;
}

void pt_insert(PieceTable *pt, const char *text, int len) {
    if (pt->add_length + len > pt->add_capacity) {
        pt->add_capacity *= 2;
        pt->add_buffer = realloc(pt->add_buffer, pt->add_capacity);
    }
    
    memcpy(pt->add_buffer + pt->add_length, text, len);
    
    if (pt->pieces_count == 0 || 
        pt->pieces[pt->pieces_count - 1].buffer != BUFFER_ADD) {
        if (pt->pieces_count >= pt->pieces_capacity) {
            pt->pieces_capacity *= 2;
            pt->pieces = realloc(pt->pieces, sizeof(Piece) * pt->pieces_capacity);
        }
        pt->pieces[pt->pieces_count++] = (Piece){
            .buffer = BUFFER_ADD,
            .start = pt->add_length,
            .length = len
        };
    } else {
        pt->pieces[pt->pieces_count - 1].length += len;
    }
    
    pt->add_length += len;
}

void pt_delete(PieceTable *pt, int len) {
    if (pt->pieces_count == 0)
        return;
    
    Piece *last = &pt->pieces[pt->pieces_count - 1];
    if (last->buffer == BUFFER_ADD && last->length >= len) {
        last->length -= len;
        pt->add_length -= len;
        if (last->length == 0) {
            pt->pieces_count--;
        }
    }
}

int calculate_tab_width(int rendered_col) {
    return TAB_STOP - (rendered_col % TAB_STOP);
}

int actual_to_rendered(const char *line, int actual_col) {
    int rendered = 0;
    for (int i = 0; i < actual_col; i++) {
        if (line[i] == '\t') {
            rendered += TAB_STOP - (rendered % TAB_STOP);
        } else {
            rendered++;
        }
    }
    return rendered;
}

int rendered_to_actual(const char *line, int rendered_col) {
    int rendered = 0;
    int actual = 0;
    
    while (rendered < rendered_col && line[actual] != '\0') {
        if (line[actual] == '\t') {
            int tab_width = TAB_STOP - (rendered % TAB_STOP);
            if (rendered + tab_width > rendered_col) {
                return actual;
            }
            rendered += tab_width;
        } else {
            rendered++;
        }
        actual++;
    }
    
    return actual;
}

void expand_tabs(const char *src, char *dst, int dst_size) {
    int rendered = 0;
    int dst_pos = 0;
    
    for (int i = 0; src[i] != '\0' && dst_pos < dst_size - 1; i++) {
        if (src[i] == '\t') {
            int tab_width = TAB_STOP - (rendered % TAB_STOP);
            for (int j = 0; j < tab_width && dst_pos < dst_size - 1; j++) {
                dst[dst_pos++] = ' ';
                rendered++;
            }
        } else {
            dst[dst_pos++] = src[i];
            rendered++;
        }
    }
    
    dst[dst_pos] = '\0';
}

int get_tab_group_start(int rendered_col) {
    return rendered_col - (rendered_col % TAB_STOP);
}

int get_tab_group_end(int rendered_col) {
    return rendered_col + (TAB_STOP - (rendered_col % TAB_STOP));
}

int get_chars_to_delete_for_backspace_tab(int rendered_col) {
    int group_start = get_tab_group_start(rendered_col);
    return rendered_col - group_start;
}

void test_conversions() {
    const char *line = "\thello\tworld";
    
    printf("Line: \\thello\\tworld\n\n");
    printf("Conversioni actual -> rendered:\n");
    for (int i = 0; i <= 13; i++) {
        printf("  actual[%d] -> rendered[%d]\n", i, actual_to_rendered(line, i));
    }
    
    printf("\nConversioni rendered -> actual:\n");
    for (int i = 0; i <= 16; i++) {
        printf("  rendered[%d] -> actual[%d]\n", i, rendered_to_actual(line, i));
    }
}

void test_expand_tabs() {
    char expanded[256];
    const char *line = "\thello\tworld";
    
    expand_tabs(line, expanded, sizeof(expanded));
    printf("expand_tabs(\"\\thello\\tworld\") = \"%s\"\n", expanded);
}

void test_backspace_calculation() {
    printf("\nCalcolo caratteri da cancellare per backspace su tab:\n");
    for (int col = 0; col < 16; col++) {
        int chars = get_chars_to_delete_for_backspace_tab(col);
        printf("  rendered_col[%d] -> delete %d char(s)\n", col, chars);
    }
}

void test_insert_tab() {
    PieceTable pt;
    pt_init(&pt);
    
    printf("\nInserimento hard tab:\n");
    
    pt_insert(&pt, "\t", 1);
    printf("  Inserito '\\t' (1 char), add_length=%d, pieces_count=%d\n",
           pt.add_length, pt.pieces_count);
    printf("  -> Il carattere '\\t' viene salvato direttamente nel buffer\n");
    
    pt_delete(&pt, 1);
    printf("  Cancellato '\\t', add_length=%d, pieces_count=%d\n",
           pt.add_length, pt.pieces_count);
    
    free(pt.add_buffer);
    free(pt.pieces);
}

int main() {
    printf("=== Hard Tab in Piece Table - Esempio ===\n\n");
    
    test_conversions();
    test_expand_tabs();
    test_backspace_calculation();
    test_insert_tab();
    
    printf("\n=== Punti chiave ===\n");
    printf("1. Il tab '\\t' e' un singolo carattere nel buffer\n");
    printf("2. La larghezza visuale varia in base alla posizione\n");
    printf("3. Tab stop a %d: 1, %d, %d, %d, ...\n",
           TAB_STOP, TAB_STOP*2, TAB_STOP*3, TAB_STOP*4);
    printf("4. Per cancellare un tab, calcola quanti caratteri 'visivi' occupa\n");
    printf("5. Il rendering richiede espansione dinamica dei tab\n");
    
    return 0;
}
