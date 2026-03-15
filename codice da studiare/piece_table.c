#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ============================================================================
 * PIECE TABLE - Struttura Dati per Editor di Testo
 * ============================================================================
 * 
 * PROBLEMA: In un editor di testo, quando inseriamo o cancelliamo caratteri,
 * dobbiamo spostare tutto il testo successivo. Questo è inefficiente per
 * documenti grandi.
 * 
 * SOLUZIONE: La Piece Table non modifica il testo esistente, ma crea
 * "pezzi" (pieces) che puntano a porzioni di buffer differenti.
 * 
 * Due buffer:
 * - Original Buffer: contiene il testo originale (mai modificato)
 * - Add Buffer: contiene tutto il nuovo testo aggiunto (append-only)
 * 
 * Ogni piece dice: "leggi X caratteri dal buffer Y a partire da Z"
 * 
 * Esempio pratico:
 *   Testo originale: "Ciao mondo"
 *   Inseriamo "bell" in posizione 5:
 *   
 *   Prima: [Piece: ORIGINAL, start=0, len=10] -> "Ciao mondo"
 *   
 *   Dopo l'inserimento:
 *   [Piece: ORIGINAL, start=0, len=5]  -> "Ciao "
 *   [Piece: ADD, start=0, len=5]       -> "bell" (nuovo testo)
 *   [Piece: ORIGINAL, start=5, len=5]  -> "mondo"
 * 
 * Vantaggi:
 * - Inserimenti rapidi: O(1) invece di O(n)
 * - Annulla/Ripeti efficiente
 * - Usato in VS Code, Atom, ecc.
 * ============================================================================
 */

/*-----------------------------------------------------------------------------
 * DEFINIZIONE DEI TIPI
 *----------------------------------------------------------------------------*/

/* Indica da quale buffer proviene il testo */
typedef enum {
    BUFFER_ORIGINAL,  /* Testo caricato dal file iniziale */
    BUFFER_ADD        /* Nuovo testo aggiunto durante la sessione */
} BufferType;

/*
 * Un "Pezzo" rappresenta una porzione contigua del documento finale.
 * Non contiene il testo direttamente, ma punta a dove trovarlo.
 */
typedef struct {
    BufferType buffer; /* Quale buffer usare: ORIGINAL o ADD */
    int start;        /* Indice iniziale nel buffer sorgente */
    int length;       /* Numero di caratteri da leggere */
} Piece;

/*
 * La Piece Table vera e propria.
 * Contiene i due buffer e l'elenco dei pezzi che formano il documento.
 */
typedef struct {
    const char *original_buffer;  /* Testo originale (read-only, da file) */
    int original_length;          /* Lunghezza del buffer originale */
    
    char *add_buffer;             /* Buffer per nuovo testo aggiunto */
    int add_capacity;             /* Capacità allocata del buffer add */
    int add_length;               /* Quanti caratteri sono stati aggiunti */
    
    Piece *pieces;                /* Array dinamico di pezzi */
    int pieces_capacity;          /* Capacità allocata per i pezzi */
    int pieces_count;             /* Numero di pezzi attuali */
} PieceTable;

/*-----------------------------------------------------------------------------
 * FUNZIONI DI SUPPORTO
 *----------------------------------------------------------------------------*/

/*
 * Stampa l'intero documento ricostruendo il testo dai pezzi.
 * Itera su tutti i pezzi e per ognuno legge i caratteri dal buffer corretto.
 */
void piece_table_print(const PieceTable *pt) {
    printf("=== CONTENUTO DEL DOCUMENTO ===\n");
    
    for (int i = 0; i < pt->pieces_count; i++) {
        Piece p = pt->pieces[i];
        
        /* Determina quale buffer usare per questo pezzo */
        const char *source = (p.buffer == BUFFER_ORIGINAL) 
            ? pt->original_buffer 
            : pt->add_buffer;
        
        /* Stampa i caratteri di questo pezzo */
        printf("[Pezzo %d: %s, start=%d, len=%d] -> \"", 
               i, 
               p.buffer == BUFFER_ORIGINAL ? "ORIGINAL" : "ADD",
               p.start, 
               p.length);
        
        for (int j = 0; j < p.length; j++) {
            putchar(source[p.start + j]);
        }
        printf("\"\n");
    }
    
    /* Stampa finale compatta */
    printf("\nDocumento: \"");
    for (int i = 0; i < pt->pieces_count; i++) {
        Piece p = pt->pieces[i];
        const char *source = (p.buffer == BUFFER_ORIGINAL) 
            ? pt->original_buffer 
            :            pt->add_buffer;
        for (int j = 0; j < p.length; j++) {
            putchar(source[p.start + j]);
        }
    }
    printf("\"\n\n");
}

/*
 * Stampa la struttura della piece table in modo schematico.
 * Utile per capire come sono organizzati i pezzi.
 */
void piece_table_print_structure(const PieceTable *pt) {
    printf("=== STRUTTURA DELLA PIECE TABLE ===\n");
    printf("Original Buffer: \"%s\" (len=%d)\n", 
           pt->original_buffer, pt->original_length);
    printf("Add Buffer: \"%s\" (len=%d/%d)\n", 
           pt->add_buffer, pt->add_length, pt->add_capacity);
    printf("Pezzi totali: %d\n\n", pt->pieces_count);
    
    /* Schema visivo della sequenza dei pezzi */
    printf("Sequenza pezzi: [");
    for (int i = 0; i < pt->pieces_count; i++) {
        if (i > 0) printf(" | ");
        printf("%s:%d:%d", 
               pt->pieces[i].buffer == BUFFER_ORIGINAL ? "O" : "A",
               pt->pieces[i].start, 
               pt->pieces[i].length);
    }
    printf("]\n\n");
}

/*-----------------------------------------------------------------------------
 * FUNZIONI PRINCIPALI DELLA PIECE TABLE
 *----------------------------------------------------------------------------*/

/*
 * Inizializza una nuova Piece Table con un documento vuoto.
 */
PieceTable* piece_table_create(void) {
    PieceTable *pt = malloc(sizeof(PieceTable));
    if (!pt) return NULL;
    
    /* Inizializza il buffer add con capacità iniziale */
    pt->add_capacity = 1024;
    pt->add_buffer = malloc(pt->add_capacity);
    pt->add_length = 0;
    pt->add_buffer[0] = '\0';
    
    /* Inizializza l'array dei pezzi */
    pt->pieces_capacity = 100;
    pt->pieces = malloc(sizeof(Piece) * pt->pieces_capacity);
    pt->pieces_count = 0;
    
    /* Inizializza buffer originale a stringa vuota */
    pt->original_buffer = "";
    pt->original_length = 0;
    
    return pt;
}

/*
 * Carica un documento esistente nella Piece Table.
 * Crea un unico piece che punta a tutto il testo originale.
 */
void piece_table_load(PieceTable *pt, const char *filename) {
    /* Simulazione: leggiamo il contenuto del file (qui è hardcoded) */
    const char *content = "Il sole tramonta dietro le montagne.\n"
                          "Gli uccelli tornano ai loro nidi.\n"
                          "La luna inizia a brillare nel cielo stellato.\n";
    
    pt->original_buffer = content;
    pt->original_length = strlen(content);
    
    /* Un solo pezzo che punta a tutto il buffer originale */
    pt->pieces[0].buffer = BUFFER_ORIGINAL;
    pt->pieces[0].start = 0;
    pt->pieces[0].length = pt->original_length;
    pt->pieces_count = 1;
}

/*
 * Inserisce testo in una posizione specifica del documento.
 * 
 * Algoritmo:
 * 1. Aggiunge il nuovo testo all'Add Buffer
 * 2. Trova il piece che contiene la posizione di inserimento
 * 3. Lo divide in due (se necessario)
 * 4. Inserisce il nuovo piece in mezzo
 * 
 * Parametri:
 *   pt: la piece table
 *   position: posizione dove inserire (0 = inizio)
 *   text: testo da inserire
 */
void piece_table_insert(PieceTable *pt, int position, const char *text) {
    int text_len = strlen(text);
    if (text_len == 0) return;
    
    /* Verifica che la posizione sia valida */
    if (position < 0) position = 0;
    
    /* Calcola la lunghezza totale del documento */
    int doc_length = 0;
    for (int i = 0; i < pt->pieces_count; i++) {
        doc_length += pt->pieces[i].length;
    }
    if (position > doc_length) position = doc_length;
    
    /* ============================================================
     * PASSO 1: Aggiungi il nuovo testo all'Add Buffer
     * ============================================================
     * L'Add Buffer cresce solo aggiungendo in coda (efficiente!)
     * Se serve più spazio, raddoppia la capacità.
     */
    if (pt->add_length + text_len + 1 > pt->add_capacity) {
        pt->add_capacity *= 2;
        pt->add_buffer = realloc(pt->add_buffer, pt->add_capacity);
    }
    
    int add_position = pt->add_length;
    memcpy(pt->add_buffer + pt->add_length, text, text_len + 1);
    pt->add_length += text_len;
    
    /* ============================================================
     * PASSO 2: Trova il piece che contiene la posizione
     * ============================================================
     * Scorri i pezzi fino a trovare quello dove inserire.
     */
    int current_pos = 0;
    int piece_index = -1;
    int offset_in_piece = 0;
    
    for (int i = 0; i < pt->pieces_count; i++) {
        int piece_end = current_pos + pt->pieces[i].length;
        
        if (position <= piece_end) {
            /* Trovato! La posizione è dentro questo piece */
            piece_index = i;
            offset_in_piece = position - current_pos;
            break;
        }
        current_pos = piece_end;
    }
    
    /* Se la posizione è alla fine, usa l'ultimo piece */
    if (piece_index == -1) {
        piece_index = pt->pieces_count - 1;
        offset_in_piece = pt->pieces[piece_index].length;
    }
    
    /* ============================================================
     * PASSO 3: Dividi il piece e inserisci il nuovo testo
     * ============================================================
     * 
     * Caso A: Inserimento a metà di un piece
     * Prima:  [Piece: "ABCDEF"]
     * Dopo:   [Piece: "ABC"] + [NEW] + [Piece: "DEF"]
     * 
     * Caso B: Inserimento all'inizio di un piece
     * Prima:  [Piece: "ABCDEF"]
     * Dopo:   [NEW] + [Piece: "ABCDEF"]
     * 
     * Caso C: Inserimento alla fine di un piece
     * Prima:  [Piece: "ABCDEF"]
     * Dopo:   [Piece: "ABCDEF"] + [NEW]
     */
    
    Piece old_piece = pt->pieces[piece_index];
    
    /* Sposta i pezzi successivi per fare spazio al nuovo piece */
    /* Se inserted a metà, servono 2 nuovi pezzi (destra + nuovo) */
    /* Se inserito a inizio/fine, serve 1 nuovo piece */
    int new_pieces_needed = (offset_in_piece > 0 && offset_in_piece < old_piece.length) ? 2 : 1;
    
    /* Assicurati che ci sia spazio nell'array */
    if (pt->pieces_count + new_pieces_needed > pt->pieces_capacity) {
        pt->pieces_capacity *= 2;
        pt->pieces = realloc(pt->pieces, sizeof(Piece) * pt->pieces_capacity);
    }
    
    /* Sposta i pezzi successivi per fare spazio */
    for (int i = pt->pieces_count; i > piece_index + new_pieces_needed; i--) {
        pt->pieces[i] = pt->pieces[i - new_pieces_needed];
    }
    
    int new_index = piece_index;
    
    /* Se c'è una parte sinistra (prima del punto di inserimento) */
    if (offset_in_piece > 0) {
        pt->pieces[new_index].buffer = old_piece.buffer;
        pt->pieces[new_index].start = old_piece.start;
        pt->pieces[new_index].length = offset_in_piece;
        new_index++;
    }
    
    /* Inserisci il nuovo piece con il testo aggiunto */
    pt->pieces[new_index].buffer = BUFFER_ADD;
    pt->pieces[new_index].start = add_position;
    pt->pieces[new_index].length = text_len;
    new_index++;
    
    /* Se c'è una parte destra (dopo il punto di inserimento) */
    if (offset_in_piece < old_piece.length) {
        pt->pieces[new_index].buffer = old_piece.buffer;
        pt->pieces[new_index].start = old_piece.start + offset_in_piece;
        pt->pieces[new_index].length = old_piece.length - offset_in_piece;
    }
    
    pt->pieces_count += new_pieces_needed;
}

/*
 * Elimina un intervallo di caratteri dal documento.
 * 
 * Parametri:
 *   pt: la piece table
 *   start: posizione iniziale da eliminare
 *   length: numero di caratteri da eliminare
 */
void piece_table_delete(PieceTable *pt, int start, int length) {
    if (length <= 0 || start < 0) return;
    
    /* Calcola la lunghezza totale */
    int doc_length = 0;
    for (int i = 0; i < pt->pieces_count; i++) {
        doc_length += pt->pieces[i].length;
    }
    
    /* Limita la cancellazione al documento */
    if (start >= doc_length) return;
    if (start + length > doc_length) {
        length = doc_length - start;
    }
    
    /* Trova il piece iniziale e finale da eliminare */
    int current_pos = 0;
    int start_piece = -1, start_offset = 0;
    int end_piece = -1, end_offset = 0;
    
    for (int i = 0; i < pt->pieces_count; i++) {
        int piece_start = current_pos;
        int piece_end = current_pos + pt->pieces[i].length;
        
        if (start_piece == -1 && start < piece_end) {
            start_piece = i;
            start_offset = start - piece_start;
        }
        
        if (end_piece == -1 && start + length <= piece_end) {
            end_piece = i;
            end_offset = start + length - piece_start;
            break;
        }
        
        current_pos = piece_end;
    }
    
    if (start_piece == -1 || end_piece == -1) return;
    
    /* ============================================================
     * Eliminazione: compattiamo i pezzi
     * ============================================================
     * 
     * Casi possibili:
     * - Stesso piece: dividilo in due parti (salva la parte che resta)
     * - Più pezzi: rimuovi quelli in mezzo, unisci i bordi
     */
    
    if (start_piece == end_piece) {
        /* Stesso piece: dividi in due */
        Piece p = pt->pieces[start_piece];
        
        if (start_offset > 0 && end_offset < p.length) {
            /*keep_left | delete | keep_right -> keep_left + keep_right*/
            pt->pieces[start_piece].length = start_offset;
            
            /* Sposta per fare spazio */
            for (int i = pt->pieces_count; i > start_piece + 1; i--) {
                pt->pieces[i] = pt->pieces[i - 1];
            }
            
            pt->pieces[start_piece + 1].buffer = p.buffer;
            pt->pieces[start_piece + 1].start = p.start + end_offset;
            pt->pieces[start_piece + 1].length = p.length - end_offset;
            pt->pieces_count++;
        } else if (start_offset == 0) {
            /* Cancella dall'inizio del piece */
            pt->pieces[start_piece].start = p.start + end_offset;
            pt->pieces[start_piece].length = p.length - end_offset;
        } else {
            /* Cancella dalla fine del piece */
            pt->pieces[start_piece].length = start_offset;
        }
    } else {
        /* Più pezzi: elimina l'inizio e la fine, rimuovi quelli in mezzo */
        
        /* Primo piece: tiene solo l'inizio */
        if (start_offset > 0) {
            pt->pieces[start_piece].length = start_offset;
        } else {
            /* Rimuovi completamente questo piece */
            for (int i = start_piece; i < end_piece; i++) {
                pt->pieces[i] = pt->pieces[i + 1];
            }
            end_piece--;
            start_piece--;
        }
        
        /* Ultimo piece: tiene solo la fine */
        Piece end_p = pt->pieces[end_piece];
        int keep_from_end = end_p.length - end_offset;
        
        if (keep_from_end > 0) {
            pt->pieces[end_piece].start = end_p.start + end_offset;
            pt->pieces[end_piece].length = keep_from_end;
        } else {
            /* Rimuovi questo piece */
            for (int i = end_piece; i < pt->pieces_count - 1; i++) {
                pt->pieces[i] = pt->pieces[i + 1];
            }
            pt->pieces_count--;
        }
        
        /* Rimuovi i pezzi in mezzo (se ce ne sono) */
        int pieces_to_remove = end_piece - start_piece - 1;
        if (pieces_to_remove > 0) {
            for (int i = start_piece + 1; i < pt->pieces_count - pieces_to_remove; i++) {
                pt->pieces[i] = pt->pieces[i + pieces_to_remove];
            }
            pt->pieces_count -= pieces_to_remove;
        }
    }
}

/*
 * Libera la memoria allocata dalla Piece Table.
 */
void piece_table_destroy(PieceTable *pt) {
    if (pt) {
        free(pt->add_buffer);
        free(pt->pieces);
        free(pt);
    }
}

/*-----------------------------------------------------------------------------
 * DEMONSTRAZIONE
 *----------------------------------------------------------------------------*/

int main(void) {
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║           DEMONSTRAZIONE DELLA PIECE TABLE                   ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n\n");
    
    /* Crea una nuova piece table */
    PieceTable *pt = piece_table_create();
    
    /* Carica un documento (simulato) */
    piece_table_load(pt, "documento.txt");
    
    printf("--- PASSO 1: Documento caricato ---\n");
    piece_table_print_structure(pt);
    piece_table_print(pt);
    
    /* ============================================================
     * PASSO 2: Inserisci "MERAVIGLIOSO " all'inizio
     * ============================================================
     * "Il sole tramonta..." -> "MERAVIGLIOSO Il sole tramonta..."
     */
    printf("--- PASSO 2: Inserisco 'MERAVIGLIOSO ' all'inizio ---\n");
    piece_table_insert(pt, 0, "MERAVIGLIOSO ");
    piece_table_print_structure(pt);
    piece_table_print(pt);
    
    /* ============================================================
     * PASSO 3: Inserisci " davvero" dopo "tramonta"
     * ============================================================
     * Trova la posizione di "tramonta" e inserisci dopo
     */
    printf("--- PASSO 3: Inserisco ' davvero' dopo 'tramonta' ---\n");
    /* La posizione di "tramonta" è a carattere 43 circa */
    piece_table_insert(pt, 43, " davvero");
    piece_table_print_structure(pt);
    piece_table_print(pt);
    
    /* ============================================================
     * PASSO 4: Cancella una parte del testo
     * ============================================================
     * Cancella "dietro le " (primi 12 caratteri dopo "tramonta")
     */
    printf("--- PASSO 4: Cancello 'dietro le ' ---\n");
    piece_table_delete(pt, 30, 12);
    piece_table_print_structure(pt);
    piece_table_print(pt);
    
    /* Cleanup */
    piece_table_destroy(pt);
    
    printf("══════════════════════════════════════════════════════════════\n");
    printf("Fine dimostrazione!\n");
    
    return 0;
}
